#pragma once

#include <memory>
#include <vector>

#include "query/parser/logical_plan/logical_plan.h"

class ProjectionPlan : public LogicalPlan {
public:
    std::unique_ptr<LogicalPlan> child;

    bool distinct;

    int64_t limit;

    std::vector<Column> columns;

    ProjectionPlan(
        std::unique_ptr<LogicalPlan> child,
        bool distinct,
        int64_t limit,
        std::vector<Column>&& columns
    ) :
        child    (std::move(child)),
        distinct (distinct),
        limit    (limit),
        columns  (std::move(columns)) { }

    void accept_visitor(LogicalPlanVisitor& visitor) override {
        visitor.visit(*this);
    }

    std::unique_ptr<LogicalPlan> clone() const override {
        std::vector<Column> columns_clone = columns;
        return std::make_unique<ProjectionPlan>(
            child->clone(),
            distinct,
            limit,
            std::move(columns_clone)
        );
    }

    std::ostream& print_to_ostream(std::ostream& os, int indent = 0) const override {
        os << std::string(indent, ' ');
        os << "Projection(" << (distinct ? "DISTINCT " : "");
        os << columns[0].alias << "." << columns[0].column;
        for (size_t i = 1; i < columns.size(); i++) {
            os << ", " << columns[i].alias << "." << columns[i].column;
        }
        if (limit != INT64_MAX) {
            os << " LIMIT " << limit;
        }
        os << ")\n";
        return child->print_to_ostream(os, indent + 2);
    }
};
