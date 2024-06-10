#pragma once

#include "query/executor/expr/expr.h"
#include "query/parser/logical_plan/logical_plan.h"

class ExprBetween : public Expr {
public:
    std::unique_ptr<Expr> child;

    Value lower_bound;
    Value upper_bound;

    ExprBetween(std::unique_ptr<Expr> child, Value&& lower_bound, Value&& upper_bound) :
        child       (std::move(child)),
        lower_bound (std::move(lower_bound)),
        upper_bound (std::move(upper_bound)),
        res         ((int64_t)0) { }

    const Value& eval() override {
        auto& child_eval = child->eval();
        bool condition = lower_bound <= child_eval && child_eval <= upper_bound;
        res.value.as_int = static_cast<int64_t>(condition);
        return res;
    }

    std::ostream& print_to_ostream(std::ostream& os) const override {
        os << *child << " BETWEEN [" << lower_bound << ", " << upper_bound << ']';
        return os;
    }

private:
    Value res;
};
