#pragma once

#include <map>
#include <memory>
#include <queue>
#include <set>
#include <stack>
#include <vector>

#include "query/parser/logical_plan/expr/expr_plans.h"
#include "query/parser/logical_plan/logical_plans.h"

namespace Parser::QueryRewriter {

// This visitor creates join nodes (only equality)
class BuildJoins : public LogicalPlanVisitor {
public:
    void visit(CartesianProductPlan& cartesian_product) override {
        assert(cartesian_product.children.size() >= 2
            && "CartesianProductPlan should have at least 2 children");

        // each group is either a JoinPlan of two or more connected Relations
        // or a RelationPlan
        std::vector<std::unique_ptr<LogicalPlan>> groups;

        auto& children = cartesian_product.children;

        while (!children.empty()) {
            auto* first_relation = static_cast<RelationPlan*>(children[0].get());
            assert(first_relation != nullptr && "Expected a relation");

            std::set<std::string> joined_aliases;
            joined_aliases.insert(first_relation->alias);

            std::queue<std::string> pending_aliases;
            pending_aliases.push(first_relation->alias);

            while (!pending_aliases.empty()) {
                auto& current_alias = pending_aliases.front();

                for (auto& column_equality : column_equalities) {
                    if (column_equality.first.alias == current_alias) {
                        auto insert_res = joined_aliases.insert(column_equality.second.alias);
                        if (insert_res.second) {
                            pending_aliases.push(column_equality.second.alias);
                        }
                    }
                    else if (column_equality.second.alias == current_alias) {
                        auto insert_res = joined_aliases.insert(column_equality.first.alias);
                        if (insert_res.second) {
                            pending_aliases.push(column_equality.first.alias);
                        }
                    }
                }

                pending_aliases.pop();
            }

            std::vector<std::pair<Column, Column>> current_join_columns;

            for (auto& column_equality : column_equalities) {
                if (joined_aliases.find(column_equality.first.alias) != joined_aliases.end()
                    || joined_aliases.find(column_equality.second.alias) != joined_aliases.end())
                {
                    current_join_columns.push_back(column_equality);
                }
            }

            std::vector<std::unique_ptr<LogicalPlan>> join_children;
            for (auto& child : children) {
                auto* relation = static_cast<RelationPlan*>(child.get());
                assert(relation != nullptr && "Expected a relation");

                if (joined_aliases.find(relation->alias) != joined_aliases.end()) {
                    join_children.push_back(std::move(child));
                }
            }

            if (join_children.size() > 1) {
                groups.push_back(std::make_unique<JoinPlan>(
                    std::move(join_children),
                    std::move(current_join_columns)
                ));
            } else {
                groups.push_back(std::move(join_children[0]));
            }

            children.erase(
                std::remove_if(children.begin(), children.end(), [](std::unique_ptr<LogicalPlan>& x) {
                    return x == nullptr;
                }),
                children.end()
            );
        }

        if (groups.size() > 1) {
            current_plan = std::make_unique<CartesianProductPlan>(
                std::move(groups)
            );
        } else {
            current_plan = std::move(groups[0]);
        }
    }


    void visit(ProjectionPlan& projection) override {
        projection.child->accept_visitor(*this);
        current_plan = std::make_unique<ProjectionPlan>(
            std::move(current_plan),
            projection.distinct,
            projection.limit,
            std::move(projection.columns)
        );
    }


    void visit(SelectionPlan& selection) override {
        std::vector<std::unique_ptr<ExprPlan>> remaining_expressions;
        for (auto& expression : selection.expressions) {
            // check for equalities of two columns
            auto* casted = dynamic_cast<ExprPlanEquals*>(expression.get());
            if (casted != nullptr) {
                auto* casted_lhs = dynamic_cast<ExprPlanColumn*>(casted->lhs.get());
                auto* casted_rhs = dynamic_cast<ExprPlanColumn*>(casted->rhs.get());

                if (casted_lhs != nullptr && casted_rhs != nullptr) {
                    column_equalities.push_back({casted_lhs->column, casted_rhs->column});
                    continue; // to avoid push back to remaining_expressions
                }
            }
            remaining_expressions.push_back(std::move(expression));
        }

        selection.child->accept_visitor(*this);

        if (!remaining_expressions.empty()) {
            current_plan = std::make_unique<SelectionPlan>(
                std::move(current_plan),
                std::move(remaining_expressions));
        }
    }


    void visit(RelationPlan& relation_plan) override {
        current_plan = relation_plan.clone();
    }


    void visit(JoinPlan&) override {
        assert(false && "Joins are not supposed to exists before BuildJoins");
    }

    std::unique_ptr<LogicalPlan> current_plan;

private:
    std::vector<std::pair<Column, Column>> column_equalities;
};
} // namespace Parser::QueryRewriter
