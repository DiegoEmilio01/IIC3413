#pragma once

#include <memory>
#include <ostream>
#include <set>

#include "query/parser/logical_plan/expr/expr_plan_visitor.h"
#include "query/parser/logical_plan/logical_plan.h"
#include "relational_model/schema.h"

class ExprPlan {
public:
    virtual ~ExprPlan() = default;

    virtual std::unique_ptr<ExprPlan> clone() const = 0;

    virtual void accept_visitor(ExprPlanVisitor&) = 0;

    virtual DataType get_datatype() const = 0;

    virtual std::set<Column> get_columns() const = 0;

    virtual std::ostream& print_to_ostream(std::ostream& os) const = 0;

    friend std::ostream& operator<<(std::ostream& os, const ExprPlan& node) {
        return node.print_to_ostream(os);
    }
};

