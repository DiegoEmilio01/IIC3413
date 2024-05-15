#pragma once

#include "exceptions/exceptions.h"

class CartesianProductPlan;
class JoinPlan;
class ProjectionPlan;
class RelationPlan;
class SelectionPlan;

class LogicalPlanVisitor {
public:
    virtual void visit(CartesianProductPlan&) {
        throw NotImplementedException("visit CartesianProductPlan");
    }

    virtual void visit(ProjectionPlan&) {
        throw NotImplementedException("visit ProjectionPlan");
    }

    virtual void visit(SelectionPlan&) {
        throw NotImplementedException("visit SelectionPlan");
    }

    virtual void visit(RelationPlan&) {
        throw NotImplementedException("visit RelationPlan");
    }

    virtual void visit(JoinPlan&) {
        throw NotImplementedException("visit JoinPlan");
    }
};
