#pragma once

#include "query/executor/query_iter.h"
#include "relational_model/relation_iter.h"

class Relation : public QueryIter {
public:
    Relation(
        std::unique_ptr<RelationIter> _child,
        const std::vector<DataType>&  _relation_datatypes,
        std::vector<Column>           _projected_columns,
        std::vector<size_t>           _projected_columns_pos
    ) :
        child                 (std::move(_child)),
        projected_columns     (std::move(_projected_columns)),
        projected_columns_pos (std::move(_projected_columns_pos)),
        child_out             (_relation_datatypes),
        out                   (projected_columns.size())
    {
        assert(projected_columns.size() == projected_columns_pos.size());
        for (size_t i = 0; i < projected_columns_pos.size(); i++) {
            out.values[i] = &child_out.values[projected_columns_pos[i]];
        }
    }


    void begin() override {
        child->begin(child_out);
    }

    bool next() override {
        return (child->next());
    }

    void reset() override {
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
        os << "Relation(";
        if (projected_columns.size() > 0) {
            os << projected_columns[0].table;
            if (projected_columns[0].table != projected_columns[0].alias) {
                os << " as " << projected_columns[0].alias;
            }
        }
        os << ")\n";
        return os;
    }

private:
    std::unique_ptr<RelationIter> child;

    std::vector<Column> projected_columns;

    std::vector<size_t> projected_columns_pos;

    Record child_out;

    RecordRef out;
};
