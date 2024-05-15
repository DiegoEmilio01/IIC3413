
// Generated from IIC3413DBParser.g4 by ANTLR 4.13.1

#pragma once


#include "antlr4-runtime.h"
#include "IIC3413DBParserVisitor.h"


/**
 * This class provides an empty implementation of IIC3413DBParserVisitor, which can be
 * extended to create a visitor which only needs to handle a subset of the available methods.
 */
class  IIC3413DBParserBaseVisitor : public IIC3413DBParserVisitor {
public:

  virtual std::any visitRoot(IIC3413DBParser::RootContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitQuery(IIC3413DBParser::QueryContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitCreateQuery(IIC3413DBParser::CreateQueryContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitInsertQuery(IIC3413DBParser::InsertQueryContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitSelectQuery(IIC3413DBParser::SelectQueryContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitSchema(IIC3413DBParser::SchemaContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitDatatype(IIC3413DBParser::DatatypeContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitSelectStatement(IIC3413DBParser::SelectStatementContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitColumnList(IIC3413DBParser::ColumnListContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitFromStatement(IIC3413DBParser::FromStatementContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitTableList(IIC3413DBParser::TableListContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitTable(IIC3413DBParser::TableContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitWhereStatement(IIC3413DBParser::WhereStatementContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitAndExpr(IIC3413DBParser::AndExprContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitSimpleExpr(IIC3413DBParser::SimpleExprContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitComparisonExpr(IIC3413DBParser::ComparisonExprContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitLikeExpr(IIC3413DBParser::LikeExprContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitColumnOrConstant(IIC3413DBParser::ColumnOrConstantContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitColumn(IIC3413DBParser::ColumnContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitConstant(IIC3413DBParser::ConstantContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitIdentifier(IIC3413DBParser::IdentifierContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitLimitStatement(IIC3413DBParser::LimitStatementContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitKeyword(IIC3413DBParser::KeywordContext *ctx) override {
    return visitChildren(ctx);
  }


};

