#pragma once

#include "query/parser/logical_plan/expr/expr_plan.h"
#include "query/parser/logical_plan/logical_plan.h"

class ExprPlanColumn : public ExprPlan {
public:
    Column column;

    ExprPlanColumn(Column column) : column(column) { }

    std::unique_ptr<ExprPlan> clone() const override {
        return std::make_unique<ExprPlanColumn>(column);
    }

    void accept_visitor(ExprPlanVisitor& visitor) override {
        visitor.visit(*this);
    }

    DataType get_datatype() const override {
        return column.datatype;
    }

    std::set<Column> get_columns() const override {
        return { column };
    }

    std::ostream& print_to_ostream(std::ostream& os) const override {
        os << column.alias << '.' << column.column;
        return os;
    }
};