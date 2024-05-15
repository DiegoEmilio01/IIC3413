#pragma once

#include <cassert>

#include "query/parser/logical_plan/logical_plan.h"

class JoinPlan : public LogicalPlan {
public:
    std::vector<std::unique_ptr<LogicalPlan>> children;

    std::vector<std::pair<Column, Column>> join_columns;

    JoinPlan(std::vector<std::unique_ptr<LogicalPlan>>&& _children,
             std::vector<std::pair<Column, Column>>&& _join_columns) :
        children (std::move(_children)),
        join_columns (std::move(_join_columns))
    {
        assert(children.size() > 1);
        assert(join_columns.size() >= 1);
    }

    void accept_visitor(LogicalPlanVisitor& visitor) override {
        visitor.visit(*this);
    }

    std::unique_ptr<LogicalPlan> clone() const override {
        std::vector<std::unique_ptr<LogicalPlan>> children_clone;
        std::vector<std::pair<Column, Column>> join_columns_clone = join_columns;

        for (auto& child : children) {
            children_clone.push_back(child->clone());
        }

        return std::make_unique<JoinPlan>(
            std::move(children_clone),
            std::move(join_columns_clone)
        );
    }

    std::ostream& print_to_ostream(std::ostream& os, int indent = 0) const override {
        os << std::string(indent, ' ');
        os << "Join(";
        os << join_columns[0].first.alias << "." << join_columns[0].first.column;
        os << " == ";
        os << join_columns[0].second.alias << "." << join_columns[0].second.column;
        for (size_t i = 1; i < join_columns.size(); ++i) {
            os << " AND ";
            os << join_columns[i].first.alias << "." << join_columns[i].first.column;
            os << " == ";
            os << join_columns[i].second.alias << "." << join_columns[i].second.column;
        }
        os << ")\n";
        for (const auto& child : children) {
            child->print_to_ostream(os, indent + 2);
        }
        return os;
    }
};
