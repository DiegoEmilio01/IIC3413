#pragma once

#include "query/parser/logical_plan/logical_plan.h"

class CartesianProductPlan : public LogicalPlan {
public:
    std::vector<std::unique_ptr<LogicalPlan>> children;

    CartesianProductPlan(std::vector<std::unique_ptr<LogicalPlan>>&& children) :
        children (std::move(children)) { }

    void accept_visitor(LogicalPlanVisitor& visitor) override {
        visitor.visit(*this);
    }

    std::unique_ptr<LogicalPlan> clone() const override {
        std::vector<std::unique_ptr<LogicalPlan>> children_clone;
        for (const auto& child : children) {
            children_clone.emplace_back(child->clone());
        }
        return std::make_unique<CartesianProductPlan>(std::move(children_clone));
    }

    std::ostream& print_to_ostream(std::ostream& os, int indent = 0) const override {
        os << std::string(indent, ' ');
        os << "CartesianProduct()\n";
        for (const auto& child : children) {
            child->print_to_ostream(os, indent + 2);
        }
        return os;
    }
};
