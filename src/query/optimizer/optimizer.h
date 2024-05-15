#pragma once

#include <map>
#include <memory>
#include <set>
#include <vector>

#include "query/executor/expr/expr.h"
#include "query/executor/query_iter.h"
#include "query/parser/logical_plan/expr/expr_plan.h"
#include "query/parser/logical_plan/expr/expr_plan_visitor.h"
#include "query/parser/logical_plan/logical_plan.h"

class ExprOptimizer : public ExprPlanVisitor {
public:
    std::vector<Column> columns;
    RecordRef& output;

    ExprOptimizer(std::vector<Column>&& columns, RecordRef& output) :
        columns(std::move(columns)), output(output) { }

    std::unique_ptr<Expr> current_expr;

    void visit(ExprPlanLike&) override;

    void visit(ExprPlanColumn&) override;
    void visit(ExprPlanTerm&) override;

    void visit(ExprPlanEquals&) override;
    void visit(ExprPlanNotEquals&) override;
    void visit(ExprPlanLess&) override;
    void visit(ExprPlanLessOrEquals&) override;
};


class Optimizer : public LogicalPlanVisitor {
public:
    static std::unique_ptr<QueryIter> create_physical_plan(std::unique_ptr<LogicalPlan>);

    std::unique_ptr<QueryIter> current_iter;

    // the plans are navigated via DFS, remembering how many usages each column have
    std::map<Column, size_t> column_usage;

    void visit(CartesianProductPlan&) override;
    void visit(JoinPlan&) override;
    void visit(ProjectionPlan&) override;
    void visit(RelationPlan&) override;
    void visit(SelectionPlan&) override;
};
