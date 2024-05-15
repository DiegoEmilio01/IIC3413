#pragma once

#include <memory>
#include <ostream>

#include "relational_model/schema.h"
#include "relational_model/value.h"

class Expr {
public:
    virtual ~Expr() = default;

    virtual const Value& eval() = 0;

    virtual std::ostream& print_to_ostream(std::ostream& os) const = 0;

    friend std::ostream& operator<<(std::ostream& os, const Expr& node) {
        return node.print_to_ostream(os);
    }
};