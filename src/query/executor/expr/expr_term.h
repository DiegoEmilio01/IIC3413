#pragma once

#include "query/executor/expr/expr.h"

class ExprTerm : public Expr {
public:
    ExprTerm(Value&& value) :
        res(std::move(value)) { }

    const Value& eval() override {
        return res;
    }

    std::ostream& print_to_ostream(std::ostream& os) const override {
        os << res;
        return os;
    }

private:
    Value res;
};