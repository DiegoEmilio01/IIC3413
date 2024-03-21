#pragma once

#include "relational_model/record.h"

class RelationIter {
public:
    virtual ~RelationIter() = default;

    virtual void begin(Record& out) = 0;

    virtual bool next() = 0;

    virtual void reset() = 0;
};
