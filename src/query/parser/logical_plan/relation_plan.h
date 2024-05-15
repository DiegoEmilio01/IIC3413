#pragma once

#include "query/parser/logical_plan/logical_plan.h"

class RelationPlan : public LogicalPlan {
public:
    std::string alias;
    std::string table;

    RelationPlan(const std::string& alias, const std::string& table) :
        alias (alias),
        table (table) { }

    void accept_visitor(LogicalPlanVisitor& visitor) override {
        visitor.visit(*this);
    }

    std::unique_ptr<LogicalPlan> clone() const override {
        return std::make_unique<RelationPlan>(alias, table);
    }

    std::ostream& print_to_ostream(std::ostream& os, int indent = 0) const override {
        os << std::string(indent, ' ');
        os << "Relation(";
        os << table;
        if (alias != table) {
            os << " AS " << alias;
        }
        return os << ")\n";
    }
};
