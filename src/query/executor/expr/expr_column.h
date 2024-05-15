#pragma once

#include "query/executor/expr/expr.h"
#include "query/parser/logical_plan/logical_plan.h"

class ExprColumn : public Expr {
public:
    ExprColumn(Column column, Value* value_ptr) :
        column(column), value_ptr(value_ptr) { }

    const Value& eval() override {
        return *value_ptr;
    }

    std::ostream& print_to_ostream(std::ostream& os) const override {
        os << column.alias << "." << column.column;
        return os;
    }

private:
    Column column;

    Value* value_ptr;
};