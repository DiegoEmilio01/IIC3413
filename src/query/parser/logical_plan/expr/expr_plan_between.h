#pragma once

#include "query/parser/logical_plan/expr/expr_plan.h"
#include "query/parser/logical_plan/expr/expr_plan_column.h"
#include "query/parser/logical_plan/logical_plan.h"
#include "relational_model/value.h"

class ExprPlanBetween : public ExprPlan {
public:
    std::unique_ptr<ExprPlanColumn> child;
    Value lower_bound;
    Value upper_bound;

    ExprPlanBetween(
        std::unique_ptr<ExprPlanColumn> child,
        Value&& lower_bound,
        Value&& upper_bound
    ) :
        child (std::move(child)),
        lower_bound (std::move(lower_bound)),
        upper_bound (std::move(upper_bound)) { }

    std::unique_ptr<ExprPlan> clone() const override {
        auto lower_bound_clone = lower_bound;
        auto upper_bound_clone = upper_bound;
        return std::make_unique<ExprPlanBetween>(
            std::make_unique<ExprPlanColumn>(child->column),
            std::move(lower_bound_clone),
            std::move(upper_bound_clone)
        );
    }

    void accept_visitor(ExprPlanVisitor& visitor) override {
        visitor.visit(*this);
    }

    DataType get_datatype() const override {
        return child->column.datatype;
    }

    std::set<Column> get_columns() const override {
        return { child->column };
    }

    std::ostream& print_to_ostream(std::ostream& os) const override {
        os << child->column.alias << '.' << child->column.column;
        os << " BETWEEN [";
        os << lower_bound;
        os << " , ";
        os << upper_bound;
        os << ']';
        return os;
    }
};