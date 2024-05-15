#pragma once

#include <vector>

#include "query/parser/logical_plan/logical_plan.h"
#include "relational_model/record.h"

class QueryIter {
public:
    virtual ~QueryIter() = default;

    virtual void begin() = 0;

    virtual bool next() = 0;

    virtual void reset() = 0;

    virtual RecordRef& get_output() = 0;

    virtual std::vector<Column> get_columns() = 0;

    virtual std::ostream& print_to_ostream(std::ostream& os, int indent = 0) const = 0;

    friend std::ostream& operator<<(std::ostream& os, const QueryIter& iter) {
        return iter.print_to_ostream(os);
    }
};
