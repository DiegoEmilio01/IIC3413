#pragma once

#include <ostream>
#include <memory>
#include <string>
#include <vector>

#include "logical_plan_visitor.h"
#include "relational_model/schema.h"

class Column {
public:
    const std::string alias;
    const std::string table;
    const std::string column;
    DataType datatype;

    Column(const std::string& alias, const std::string& table, const std::string& column, DataType datatype) :
        alias(alias), table(table), column(column), datatype(datatype) { }

    bool operator==(const Column& other) const {
        // not considering datatype, as it is a dependency on the other two
        return alias == other.alias && column == other.column;
    }

    bool operator<(const Column& other) const {
        // not considering datatype, as it is a dependency on the other two
        if (alias < other.alias) {
            return true;
        } else if (other.alias < alias) {
            return false;
        } else {
            return column < other.column;
        }
    }

    bool operator!=(const Column& other) const {
        return !(*this == other);
    }
};

class LogicalPlan {
public:
    virtual ~LogicalPlan() = default;

    virtual void accept_visitor(LogicalPlanVisitor& visitor) = 0;

    virtual std::unique_ptr<LogicalPlan> clone() const = 0;

    virtual std::ostream& print_to_ostream(std::ostream& os, int indent = 0) const = 0;

    friend std::ostream& operator<<(std::ostream& os, const LogicalPlan& node) {
        return node.print_to_ostream(os);
    }
};
