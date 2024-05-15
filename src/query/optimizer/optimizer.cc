#include "optimizer.h"

#include "query/executor/cartesian_product.h"
#include "query/executor/expr/exprs.h"
#include "query/executor/join.h"
#include "query/executor/projection.h"
#include "query/executor/relation.h"
#include "query/executor/selection.h"
#include "query/parser/logical_plan/expr/expr_plans.h"
#include "query/parser/logical_plan/logical_plans.h"
#include "relational_model/system.h"
#include "storage/heap_file/heap_file_page.h"
#include "storage/heap_file/heap_file_iter.h"


std::unique_ptr<QueryIter> Optimizer::create_physical_plan(std::unique_ptr<LogicalPlan> logical_plan) {
    Optimizer optimizer;
    logical_plan->accept_visitor(optimizer);
    return std::move(optimizer.current_iter);
}


void Optimizer::visit(CartesianProductPlan& cartesian_product) {
    assert(cartesian_product.children.size() >= 2);

    cartesian_product.children[0]->accept_visitor(*this);

    for (size_t i = 1; i < cartesian_product.children.size(); i++) {
        std::vector<Column>                    projected_lhs_columns;
        std::vector<Column>                    projected_rhs_columns;
        std::vector<size_t>                    projected_lhs_columns_pos;
        std::vector<size_t>                    projected_rhs_columns_pos;
        std::vector<std::pair<size_t, size_t>> equalities;

        auto saved_lhs = std::move(current_iter);

        cartesian_product.children[i]->accept_visitor(*this);
        auto saved_rhs = std::move(current_iter);

        auto lhs_cols = saved_lhs->get_columns();
        auto rhs_cols = saved_rhs->get_columns();

        for (size_t i = 0; i < lhs_cols.size(); i++) {
            if (column_usage[lhs_cols[i]] > 0) {
                projected_lhs_columns.push_back(lhs_cols[i]);
                projected_lhs_columns_pos.push_back(i);
            }
        }
        for (size_t i = 0; i < rhs_cols.size(); i++) {
            if (column_usage[rhs_cols[i]] > 0) {
                projected_rhs_columns.push_back(rhs_cols[i]);
                projected_rhs_columns_pos.push_back(i);
            }
        }

        current_iter = std::make_unique<CartesianProduct>(
            std::move(saved_lhs),
            std::move(saved_rhs),
            std::move(projected_lhs_columns),
            std::move(projected_rhs_columns),
            std::move(projected_lhs_columns_pos),
            std::move(projected_rhs_columns_pos)
        );
    }
}


void Optimizer::visit(ProjectionPlan& projection) {
    if (projection.distinct) {
        throw NotImplementedException("Projection with DISTINCT");
    }

    for (const auto& column : projection.columns) {
        column_usage[column] += 1;
    }

    projection.child->accept_visitor(*this);

    current_iter = std::make_unique<Projection>(
        std::move(current_iter),
        std::move(projection.columns),
        projection.limit
    );
}


void Optimizer::visit(SelectionPlan& selection) {
    for (auto& e : selection.expressions) {
        for (auto& column: e->get_columns()) {
            column_usage[column] += 1;
        }
    }

    selection.child->accept_visitor(*this);

    std::vector<std::unique_ptr<Expr>> expressions;
    for (auto& e : selection.expressions) {
        for (auto& column: e->get_columns()) {
            column_usage[column] -= 1;
        }

        ExprOptimizer expr_visitor(current_iter->get_columns(), current_iter->get_output());
        e->accept_visitor(expr_visitor);
        expressions.push_back(std::move(expr_visitor.current_expr));
    }

    if (expressions.size() > 0) {
        current_iter = std::make_unique<Selection>(
            std::move(current_iter),
            std::move(expressions)
        );
    }
}


void Optimizer::visit(RelationPlan& relation) {
    Schema schema;
    auto table = catalog.get_table(relation.table, &schema);
    assert(table != nullptr);

    std::vector<Column> projected_columns;
    std::vector<size_t> projected_columns_pos;

    for (size_t i = 0; i < schema.datatypes.size(); i++) {
        Column col(
            relation.alias,
            relation.table,
            schema.column_names[i],
            schema.datatypes[i]
        );

        if (column_usage[col] > 0) {
            projected_columns.push_back(col);
            projected_columns_pos.push_back(i);
        }
    }


    std::unique_ptr<RelationIter> relation_iter = table->get_record_iter();

    current_iter = std::make_unique<Relation>(
        std::move(relation_iter),
        schema.datatypes,
        std::move(projected_columns),
        std::move(projected_columns_pos)
    );
}


void Optimizer::visit(JoinPlan& join) {
    for (const auto& join_column : join.join_columns) {
        column_usage[join_column.first]  += 1;
        column_usage[join_column.second] += 1;
    }

    join.children[0]->accept_visitor(*this);

    for (size_t i = 1; i < join.children.size(); i++) {
        std::vector<Column>                    projected_lhs_columns;
        std::vector<Column>                    projected_rhs_columns;
        std::vector<size_t>                    projected_lhs_columns_pos;
        std::vector<size_t>                    projected_rhs_columns_pos;
        std::vector<std::pair<size_t, size_t>> equalities;

        auto saved_lhs = std::move(current_iter);

        join.children[1]->accept_visitor(*this);
        auto saved_rhs = std::move(current_iter);

        auto lhs_cols = saved_lhs->get_columns();
        auto rhs_cols = saved_rhs->get_columns();

        for (size_t i = 0; i < lhs_cols.size(); i++) {
            if (column_usage[lhs_cols[i]] > 0) {
                projected_lhs_columns.push_back(lhs_cols[i]);
                projected_lhs_columns_pos.push_back(i);
            }
        }

        for (size_t i = 0; i < rhs_cols.size(); i++) {
            if (column_usage[rhs_cols[i]] > 0) {
                projected_rhs_columns.push_back(rhs_cols[i]);
                projected_rhs_columns_pos.push_back(i);
            }
        }

        for (auto& col_pair : join.join_columns) {
            auto lhs_col = col_pair.first;
            auto rhs_col = col_pair.second;

            int lhs_col_pos = -1;
            int rhs_col_pos = -1;

            for (size_t i = 0; i < lhs_cols.size(); i++) {
                if (lhs_cols[i] == lhs_col) {
                    lhs_col_pos = i;
                    break;
                }
            }

            for (size_t i = 0; i < rhs_cols.size(); i++) {
                if (rhs_cols[i] == rhs_col) {
                    rhs_col_pos = i;
                    break;
                }
            }

            if (lhs_col_pos != -1 && rhs_col_pos != -1) {
                equalities.push_back({ lhs_col_pos, rhs_col_pos });
            }
        }


        current_iter = std::make_unique<Join>(
            std::move(saved_lhs),
            std::move(saved_rhs),
            std::move(projected_lhs_columns),
            std::move(projected_rhs_columns),
            std::move(projected_lhs_columns_pos),
            std::move(projected_rhs_columns_pos),
            std::move(equalities)
        );

    }
    for (const auto& join_column : join.join_columns) {
        column_usage[join_column.first]  -= 1;
        column_usage[join_column.second] -= 1;
    }
}


void ExprOptimizer::visit(ExprPlanColumn& e) {
    int index = -1;
    for (size_t i = 0; i < columns.size(); i++) {
        if (e.column == columns[i]) {
            index = i;
            break;
        }
    }
    assert(index != -1);
    current_expr = std::make_unique<ExprColumn>(
        e.column,
        output.values[index]
    );
}


void ExprOptimizer::visit(ExprPlanTerm& e) {
    current_expr = std::make_unique<ExprTerm>(
        std::move(e.value)
    );
}


void ExprOptimizer::visit(ExprPlanEquals& e) {
    e.lhs->accept_visitor(*this);
    auto lhs = std::move(current_expr);

    e.rhs->accept_visitor(*this);
    auto rhs = std::move(current_expr);

    current_expr = std::make_unique<ExprEquals>(
        std::move(lhs),
        std::move(rhs)
    );
}


void ExprOptimizer::visit(ExprPlanNotEquals& e) {
    e.lhs->accept_visitor(*this);
    auto lhs = std::move(current_expr);

    e.rhs->accept_visitor(*this);
    auto rhs = std::move(current_expr);

    current_expr = std::make_unique<ExprNotEquals>(
        std::move(lhs),
        std::move(rhs)
    );
}


void ExprOptimizer::visit(ExprPlanLess& e) {
    e.lhs->accept_visitor(*this);
    auto lhs = std::move(current_expr);

    e.rhs->accept_visitor(*this);
    auto rhs = std::move(current_expr);

    current_expr = std::make_unique<ExprLess>(
        std::move(lhs),
        std::move(rhs)
    );
}


void ExprOptimizer::visit(ExprPlanLessOrEquals& e) {
    e.lhs->accept_visitor(*this);
    auto lhs = std::move(current_expr);

    e.rhs->accept_visitor(*this);
    auto rhs = std::move(current_expr);

    current_expr = std::make_unique<ExprLessOrEquals>(
        std::move(lhs),
        std::move(rhs)
    );
}


void ExprOptimizer::visit(ExprPlanLike& e) {
    e.column->accept_visitor(*this);

    current_expr = std::make_unique<ExprLike>(
        std::move(current_expr),
        std::move(e.pattern)
    );
}
