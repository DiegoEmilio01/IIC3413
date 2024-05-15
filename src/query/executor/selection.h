#pragma once

#include "query/executor/expr/expr.h"
#include "query/executor/query_iter.h"

class Selection : public QueryIter {
public:
    Selection(
        std::unique_ptr<QueryIter> _child,
        std::vector<std::unique_ptr<Expr>> _expressions
    ) :
        child       (std::move(_child)),
        expressions (std::move(_expressions)),
        child_out   (child->get_output()) { }


    void begin() override {
        child->begin();
    }

    bool next() override {
        while (child->next()) {
            bool expr_evaluation = true;
            for (auto& e : expressions) {
                if (e->eval().value.as_int == 0) {
                    expr_evaluation = false;
                    break;
                }
            }
            if (expr_evaluation) {
                return true;
            }
        }
        return false;
    }

    void reset() override {
        child->reset();
    }

    RecordRef& get_output() override {
        return child_out;
    }

    std::vector<Column> get_columns() override {
        return child->get_columns();
    }

    std::ostream& print_to_ostream(std::ostream& os, int indent = 0) const override {
        os << std::string(indent, ' ');
        os << "Selection(";
        os << *expressions[0];
        for (size_t i = 1; i < expressions.size(); i++) {
            os << " AND ";
            os << *expressions[i];
        }
        os << ")\n";
        child->print_to_ostream(os, indent + 2);
        return os;
    }

private:
    std::unique_ptr<QueryIter> child;

    std::vector<std::unique_ptr<Expr>> expressions;

    RecordRef& child_out;
};
