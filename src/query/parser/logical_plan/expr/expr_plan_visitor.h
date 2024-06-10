#pragma once

#include "exceptions/exceptions.h"

class ExprPlanBetween;
class ExprPlanLike;

class ExprPlanColumn;
class ExprPlanTerm;

class ExprPlanEquals;
class ExprPlanNotEquals;
class ExprPlanLessOrEquals;
class ExprPlanLess;

class ExprPlanVisitor {
public:
    virtual ~ExprPlanVisitor() = default;

    virtual void visit(ExprPlanColumn&)          { throw NotImplementedException("visit ExprPlanColumn"); }
    virtual void visit(ExprPlanTerm&)            { throw NotImplementedException("visit ExprPlanTerm"); }

    virtual void visit(ExprPlanEquals&)          { throw NotImplementedException("visit ExprPlanEquals"); }
    virtual void visit(ExprPlanNotEquals&)       { throw NotImplementedException("visit ExprPlanNotEquals"); }
    virtual void visit(ExprPlanLess&)            { throw NotImplementedException("visit ExprPlanLess"); }
    virtual void visit(ExprPlanLessOrEquals&)    { throw NotImplementedException("visit ExprPlanLessOrEquals"); }

    virtual void visit(ExprPlanBetween&)         { throw NotImplementedException("visit ExprPlanBetween"); }
    virtual void visit(ExprPlanLike&)            { throw NotImplementedException("visit ExprPlanLike"); }
};
