#pragma once

#include "query/parser/logical_plan/expr/expr_plan.h"
#include "query/parser/logical_plan/expr/expr_plan_column.h"
#include "query/parser/logical_plan/logical_plan.h"

class ExprPlanLike : public ExprPlan {
public:
    std::unique_ptr<ExprPlanColumn> column;
    std::string pattern;

    ExprPlanLike(std::unique_ptr<ExprPlanColumn> column, std::string&& pattern) :
        column  (std::move(column)),
        pattern (std::move(pattern)) { }

    std::unique_ptr<ExprPlan> clone() const override {
        auto pattern_clone = pattern;
        return std::make_unique<ExprPlanLike>(
            std::make_unique<ExprPlanColumn>(column->column),
            std::move(pattern_clone)
        );
    }

    void accept_visitor(ExprPlanVisitor& visitor) override {
        visitor.visit(*this);
    }

    DataType get_datatype() const override {
        return column->column.datatype;
    }

    std::set<Column> get_columns() const override {
        return { column->column };
    }

    std::ostream& print_to_ostream(std::ostream& os) const override {
        os << column->column.alias << '.' << column->column.column;
        os << " LIKE \"";
        os << pattern;
        os << '"';
        return os;
    }
};