
// Generated from IIC3413DBParser.g4 by ANTLR 4.13.1

#pragma once


#include "antlr4-runtime.h"
#include "IIC3413DBParser.h"



/**
 * This class defines an abstract visitor for a parse tree
 * produced by IIC3413DBParser.
 */
class  IIC3413DBParserVisitor : public antlr4::tree::AbstractParseTreeVisitor {
public:

  /**
   * Visit parse trees produced by IIC3413DBParser.
   */
    virtual std::any visitRoot(IIC3413DBParser::RootContext *context) = 0;

    virtual std::any visitQuery(IIC3413DBParser::QueryContext *context) = 0;

    virtual std::any visitCreateQuery(IIC3413DBParser::CreateQueryContext *context) = 0;

    virtual std::any visitInsertQuery(IIC3413DBParser::InsertQueryContext *context) = 0;

    virtual std::any visitSelectQuery(IIC3413DBParser::SelectQueryContext *context) = 0;

    virtual std::any visitSchema(IIC3413DBParser::SchemaContext *context) = 0;

    virtual std::any visitDatatype(IIC3413DBParser::DatatypeContext *context) = 0;

    virtual std::any visitSelectStatement(IIC3413DBParser::SelectStatementContext *context) = 0;

    virtual std::any visitColumnList(IIC3413DBParser::ColumnListContext *context) = 0;

    virtual std::any visitFromStatement(IIC3413DBParser::FromStatementContext *context) = 0;

    virtual std::any visitTableList(IIC3413DBParser::TableListContext *context) = 0;

    virtual std::any visitTable(IIC3413DBParser::TableContext *context) = 0;

    virtual std::any visitWhereStatement(IIC3413DBParser::WhereStatementContext *context) = 0;

    virtual std::any visitAndExpr(IIC3413DBParser::AndExprContext *context) = 0;

    virtual std::any visitSimpleExpr(IIC3413DBParser::SimpleExprContext *context) = 0;

    virtual std::any visitComparisonExpr(IIC3413DBParser::ComparisonExprContext *context) = 0;

    virtual std::any visitLikeExpr(IIC3413DBParser::LikeExprContext *context) = 0;

    virtual std::any visitColumnOrConstant(IIC3413DBParser::ColumnOrConstantContext *context) = 0;

    virtual std::any visitColumn(IIC3413DBParser::ColumnContext *context) = 0;

    virtual std::any visitConstant(IIC3413DBParser::ConstantContext *context) = 0;

    virtual std::any visitIdentifier(IIC3413DBParser::IdentifierContext *context) = 0;

    virtual std::any visitLimitStatement(IIC3413DBParser::LimitStatementContext *context) = 0;

    virtual std::any visitKeyword(IIC3413DBParser::KeywordContext *context) = 0;


};

