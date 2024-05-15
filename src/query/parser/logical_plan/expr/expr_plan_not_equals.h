#pragma once

#include "query/parser/logical_plan/expr/expr_plan.h"

class ExprPlanNotEquals : public ExprPlan {
public:
    std::unique_ptr<ExprPlan> lhs;

    std::unique_ptr<ExprPlan> rhs;

    ExprPlanNotEquals(std::unique_ptr<ExprPlan> lhs, std::unique_ptr<ExprPlan> rhs) :
        lhs(std::move(lhs)), rhs(std::move(rhs)) { }

    std::unique_ptr<ExprPlan> clone() const override {
        return std::make_unique<ExprPlanNotEquals>(lhs->clone(), rhs->clone());
    }

    void accept_visitor(ExprPlanVisitor& visitor) override {
        visitor.visit(*this);
    }

    DataType get_datatype() const override {
        // system does not support bool, so we interpret 0 as false, 1 as true
        return DataType::INT;
    }

    std::set<Column> get_columns() const override {
        auto res = lhs->get_columns();
        for (auto rhs_col : rhs->get_columns()) {
            res.insert(rhs_col);
        }
        return res;
    }

    std::ostream& print_to_ostream(std::ostream& os) const override {
        os << *lhs;
        os << " != ";
        os << *rhs;
        return os;
    }
};