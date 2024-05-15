#pragma once

#include "query/executor/query_iter.h"

class Join : public QueryIter {
public:
    Join(
        std::unique_ptr<QueryIter>             _lhs,
        std::unique_ptr<QueryIter>             _rhs,
        std::vector<Column>                    _projected_lhs_columns,
        std::vector<Column>                    _projected_rhs_columns,
        std::vector<size_t>                    _projected_lhs_columns_pos,
        std::vector<size_t>                    _projected_rhs_columns_pos,
        std::vector<std::pair<size_t, size_t>> _equalities
    ) :
        lhs                       (std::move(_lhs)),
        rhs                       (std::move(_rhs)),
        projected_lhs_columns     (std::move(_projected_lhs_columns)),
        projected_rhs_columns     (std::move(_projected_rhs_columns)),
        projected_lhs_columns_pos (std::move(_projected_lhs_columns_pos)),
        projected_rhs_columns_pos (std::move(_projected_rhs_columns_pos)),
        equalities                (std::move(_equalities)),
        lhs_out                   (lhs->get_output()),
        rhs_out                   (rhs->get_output()),
        out                       (projected_lhs_columns.size() + projected_rhs_columns.size())
    {
        assert(projected_lhs_columns.size() == projected_lhs_columns_pos.size());
        assert(projected_rhs_columns.size() == projected_rhs_columns_pos.size());

        for (size_t i = 0; i < projected_lhs_columns_pos.size(); i++) {
            out.values[i] = lhs_out.values[projected_lhs_columns_pos[i]];
        }

        size_t offset = projected_lhs_columns_pos.size();
        for (size_t i = 0; i < projected_rhs_columns_pos.size(); i++) {
            out.values[offset + i] = rhs_out.values[projected_rhs_columns_pos[i]];
        }
    }


    void begin() override {
        lhs->begin();
        rhs->begin();

        valid_lhs = false;
    }

    // simple nested loop join
    bool next() override {
        while (true) {
            if (valid_lhs) {
                while (rhs->next()) {
                    // if compatible return true, else continue
                    bool compatible = true;
                    for (auto& eq: equalities) {
                        if (*lhs_out.values[eq.first] != *rhs_out.values[eq.second]) {
                            compatible = false;
                            break;
                        }
                    }
                    if (compatible) return true;
                }
                valid_lhs = false;
            } else {
                if (lhs->next()) {
                    valid_lhs = true;
                    rhs->reset();
                } else {
                    return false;
                }
            }
        }
        return false;
    }

    void reset() override {
        lhs->reset();
        rhs->reset();
    }

    RecordRef& get_output() override {
        return out;
    }

    std::vector<Column> get_columns() override {
        std::vector<Column> res = projected_lhs_columns;

        for (auto& c : projected_rhs_columns) {
            res.push_back(c);
        }
        return res;
    }

    std::ostream& print_to_ostream(std::ostream& os, int indent = 0) const override {
        os << std::string(indent, ' ');
        os << "Join(";
        os << projected_lhs_columns[equalities[0].first].alias << "." << projected_lhs_columns[equalities[0].first].column;
        os << " == ";
        os << projected_rhs_columns[equalities[0].second].alias << "." << projected_rhs_columns[equalities[0].second].column;

        for (size_t i = 1; i < equalities.size(); ++i) {
            os << " AND ";
            os << projected_lhs_columns[equalities[1].first].alias << "." << projected_lhs_columns[equalities[1].first].column;
            os << " == ";
            os << projected_rhs_columns[equalities[1].second].alias << "." << projected_rhs_columns[equalities[1].second].column;
        }
        os << ")\n";
        lhs->print_to_ostream(os, indent + 2);
        rhs->print_to_ostream(os, indent + 2);
        return os;
    }

private:
    std::unique_ptr<QueryIter> lhs;

    std::unique_ptr<QueryIter> rhs;

    std::vector<Column> projected_lhs_columns;

    std::vector<Column> projected_rhs_columns;

    std::vector<size_t> projected_lhs_columns_pos;

    std::vector<size_t> projected_rhs_columns_pos;

    std::vector<std::pair<size_t, size_t>> equalities;

    RecordRef& lhs_out;

    RecordRef& rhs_out;

    RecordRef out;

    bool valid_lhs;
};
