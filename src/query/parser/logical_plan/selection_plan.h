#pragma once

#include <memory>
#include <vector>

#include "query/parser/logical_plan/expr/expr_plan.h"
#include "query/parser/logical_plan/logical_plan.h"

class SelectionPlan : public LogicalPlan {
public:
    std::unique_ptr<LogicalPlan> child;

    std::vector<std::unique_ptr<ExprPlan>> expressions;

    SelectionPlan(
        std::unique_ptr<LogicalPlan> _child,
        std::vector<std::unique_ptr<ExprPlan>>&& _expressions
    ) :
        child (std::move(_child)),
        expressions (std::move(_expressions))
    {
        assert(child != nullptr);
    }

    void accept_visitor(LogicalPlanVisitor& visitor) override {
        visitor.visit(*this);
    }

    std::unique_ptr<LogicalPlan> clone() const override {
        std::vector<std::unique_ptr<ExprPlan>> expressions_clone;
        for (const auto& expression : expressions) {
            expressions_clone.push_back(expression->clone());
        }
        return std::make_unique<SelectionPlan>(child->clone(), std::move(expressions_clone));
    }

    std::ostream& print_to_ostream(std::ostream& os, int indent = 0) const override {
        os << std::string(indent, ' ');
        os << "Selection(";
        os << *expressions[0];
        for (size_t i = 1; i < expressions.size(); i++) {
            os << " AND " << *expressions[i];
        }
        os << ")\n";
        child->print_to_ostream(os, indent + 2);
        return os;
    }
};
