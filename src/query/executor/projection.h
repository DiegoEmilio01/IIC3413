#pragma once

#include "query/executor/query_iter.h"

class Projection : public QueryIter {
public:
    Projection(
        std::unique_ptr<QueryIter> _child,
        std::vector<Column>        _projected_columns,
        uint64_t                   _limit
    ) :
        child             (std::move(_child)),
        projected_columns (std::move(_projected_columns)),
        limit             (_limit),
        out               (projected_columns.size())
    {
        auto child_cols = child->get_columns();
        auto& child_out = child->get_output();
        for (size_t i = 0; i < projected_columns.size(); i++) {
            for (auto& child_col : child_cols) {
                if (child_col == projected_columns[i]) {
                    out.values[i] = child_out.values[i];
                }
            }
            assert(out.values[i] != nullptr);
        }
    }


    void begin() override {
        result_count = 0;
        child->begin();
    }

    bool next() override {
        while (child->next() && result_count < limit) {
            result_count++;
            return true;
        }
        return false;
    }

    void reset() override {
        result_count = 0;
        child->reset();
    }

    RecordRef& get_output() override {
        return out;
    }

    std::vector<Column> get_columns() override {
        return projected_columns;
    }

    std::ostream& print_to_ostream(std::ostream& os, int indent = 0) const override {
        os << std::string(indent, ' ');
        os << "Projection(";
        os << projected_columns[0].alias << "." << projected_columns[0].column;
        for (size_t i = 1; i < projected_columns.size(); i++) {
            os << ", " << projected_columns[i].alias << "." << projected_columns[i].column;
        }
        os << ")\n";
        child->print_to_ostream(os, indent + 2);
        return os;
    }

private:
    std::unique_ptr<QueryIter> child;

    std::vector<Column> projected_columns;

    const uint64_t limit;

    RecordRef out;

    uint64_t result_count;
};
