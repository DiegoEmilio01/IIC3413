#pragma once

#include "query/executor/expr/expr.h"

class ExprLess : public Expr {
public:
    std::unique_ptr<Expr> lhs;

    std::unique_ptr<Expr> rhs;

    ExprLess(std::unique_ptr<Expr> lhs, std::unique_ptr<Expr> rhs) :
        lhs(std::move(lhs)), rhs(std::move(rhs)), res((int64_t)0) { }

    const Value& eval() override {
        auto& lhs_res = lhs->eval();
        auto& rhs_res = rhs->eval();

        res.value.as_int = static_cast<int64_t>(lhs_res < rhs_res);
        return res;
    }

    std::ostream& print_to_ostream(std::ostream& os) const override {
        os << *lhs << " < " << *rhs;
        return os;
    }

private:
    Value res;
};