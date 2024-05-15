#pragma once

#include "query/executor/expr/expr.h"
#include "query/parser/logical_plan/logical_plan.h"

class ExprLike : public Expr {
public:
    ExprLike(std::unique_ptr<Expr> child, std::string&& _pattern) :
        child   (std::move(child)),
        pattern (std::move(_pattern)),
        res     ((int64_t)0) { }

    const Value& eval() override {
        // TODO: implement, for now this always returns false
        res.value.as_int = static_cast<int64_t>(0);
        return res;
    }

    std::ostream& print_to_ostream(std::ostream& os) const override {
        os << *child << " LIKE \"" << pattern << '"';
        return os;
    }

private:
    std::unique_ptr<Expr> child;

    std::string pattern;

    Value res;
};