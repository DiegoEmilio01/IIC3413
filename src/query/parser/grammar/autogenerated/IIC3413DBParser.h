
// Generated from IIC3413DBParser.g4 by ANTLR 4.13.1

#pragma once


#include "antlr4-runtime.h"




class  IIC3413DBParser : public antlr4::Parser {
public:
  enum {
    AND = 1, AS = 2, CREATE = 3, DISTINCT = 4, FROM = 5, INSERT = 6, INTO = 7, 
    INT = 8, LIKE = 9, LIMIT = 10, OR = 11, SELECT = 12, STR = 13, TABLE = 14, 
    VALUES = 15, WHERE = 16, EQ = 17, NE = 18, GT = 19, LT = 20, GE = 21, 
    LE = 22, COMMA = 23, ASTERISK = 24, MINUS = 25, SLASH = 26, PLUS = 27, 
    DOT = 28, L_PAR = 29, R_PAR = 30, INTEGER = 31, STRING = 32, IDENTIFIER = 33, 
    WHITE_SPACE = 34, COMMENT_INPUT = 35, LINE_COMMENT = 36, UNRECOGNIZED = 37
  };

  enum {
    RuleRoot = 0, RuleQuery = 1, RuleCreateQuery = 2, RuleInsertQuery = 3, 
    RuleSelectQuery = 4, RuleSchema = 5, RuleDatatype = 6, RuleSelectStatement = 7, 
    RuleColumnList = 8, RuleFromStatement = 9, RuleTableList = 10, RuleTable = 11, 
    RuleWhereStatement = 12, RuleAndExpr = 13, RuleSimpleExpr = 14, RuleComparisonExpr = 15, 
    RuleLikeExpr = 16, RuleColumnOrConstant = 17, RuleColumn = 18, RuleConstant = 19, 
    RuleIdentifier = 20, RuleLimitStatement = 21, RuleKeyword = 22
  };

  explicit IIC3413DBParser(antlr4::TokenStream *input);

  IIC3413DBParser(antlr4::TokenStream *input, const antlr4::atn::ParserATNSimulatorOptions &options);

  ~IIC3413DBParser() override;

  std::string getGrammarFileName() const override;

  const antlr4::atn::ATN& getATN() const override;

  const std::vector<std::string>& getRuleNames() const override;

  const antlr4::dfa::Vocabulary& getVocabulary() const override;

  antlr4::atn::SerializedATNView getSerializedATN() const override;


  class RootContext;
  class QueryContext;
  class CreateQueryContext;
  class InsertQueryContext;
  class SelectQueryContext;
  class SchemaContext;
  class DatatypeContext;
  class SelectStatementContext;
  class ColumnListContext;
  class FromStatementContext;
  class TableListContext;
  class TableContext;
  class WhereStatementContext;
  class AndExprContext;
  class SimpleExprContext;
  class ComparisonExprContext;
  class LikeExprContext;
  class ColumnOrConstantContext;
  class ColumnContext;
  class ConstantContext;
  class IdentifierContext;
  class LimitStatementContext;
  class KeywordContext; 

  class  RootContext : public antlr4::ParserRuleContext {
  public:
    RootContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    QueryContext *query();
    antlr4::tree::TerminalNode *EOF();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  RootContext* root();

  class  QueryContext : public antlr4::ParserRuleContext {
  public:
    QueryContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    CreateQueryContext *createQuery();
    InsertQueryContext *insertQuery();
    SelectQueryContext *selectQuery();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  QueryContext* query();

  class  CreateQueryContext : public antlr4::ParserRuleContext {
  public:
    CreateQueryContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *CREATE();
    antlr4::tree::TerminalNode *TABLE();
    IdentifierContext *identifier();
    antlr4::tree::TerminalNode *L_PAR();
    std::vector<SchemaContext *> schema();
    SchemaContext* schema(size_t i);
    antlr4::tree::TerminalNode *R_PAR();
    std::vector<antlr4::tree::TerminalNode *> COMMA();
    antlr4::tree::TerminalNode* COMMA(size_t i);


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  CreateQueryContext* createQuery();

  class  InsertQueryContext : public antlr4::ParserRuleContext {
  public:
    InsertQueryContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *INSERT();
    antlr4::tree::TerminalNode *INTO();
    IdentifierContext *identifier();
    antlr4::tree::TerminalNode *VALUES();
    antlr4::tree::TerminalNode *L_PAR();
    std::vector<ConstantContext *> constant();
    ConstantContext* constant(size_t i);
    antlr4::tree::TerminalNode *R_PAR();
    std::vector<antlr4::tree::TerminalNode *> COMMA();
    antlr4::tree::TerminalNode* COMMA(size_t i);


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  InsertQueryContext* insertQuery();

  class  SelectQueryContext : public antlr4::ParserRuleContext {
  public:
    SelectQueryContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    SelectStatementContext *selectStatement();
    FromStatementContext *fromStatement();
    WhereStatementContext *whereStatement();
    LimitStatementContext *limitStatement();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  SelectQueryContext* selectQuery();

  class  SchemaContext : public antlr4::ParserRuleContext {
  public:
    SchemaContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    IdentifierContext *identifier();
    DatatypeContext *datatype();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  SchemaContext* schema();

  class  DatatypeContext : public antlr4::ParserRuleContext {
  public:
    DatatypeContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *STR();
    antlr4::tree::TerminalNode *INT();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  DatatypeContext* datatype();

  class  SelectStatementContext : public antlr4::ParserRuleContext {
  public:
    SelectStatementContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *SELECT();
    antlr4::tree::TerminalNode *ASTERISK();
    ColumnListContext *columnList();
    antlr4::tree::TerminalNode *DISTINCT();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  SelectStatementContext* selectStatement();

  class  ColumnListContext : public antlr4::ParserRuleContext {
  public:
    ColumnListContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<ColumnContext *> column();
    ColumnContext* column(size_t i);
    std::vector<antlr4::tree::TerminalNode *> COMMA();
    antlr4::tree::TerminalNode* COMMA(size_t i);


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  ColumnListContext* columnList();

  class  FromStatementContext : public antlr4::ParserRuleContext {
  public:
    FromStatementContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *FROM();
    TableListContext *tableList();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  FromStatementContext* fromStatement();

  class  TableListContext : public antlr4::ParserRuleContext {
  public:
    TableListContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<TableContext *> table();
    TableContext* table(size_t i);
    std::vector<antlr4::tree::TerminalNode *> COMMA();
    antlr4::tree::TerminalNode* COMMA(size_t i);


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  TableListContext* tableList();

  class  TableContext : public antlr4::ParserRuleContext {
  public:
    TableContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<IdentifierContext *> identifier();
    IdentifierContext* identifier(size_t i);
    antlr4::tree::TerminalNode *AS();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  TableContext* table();

  class  WhereStatementContext : public antlr4::ParserRuleContext {
  public:
    WhereStatementContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *WHERE();
    AndExprContext *andExpr();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  WhereStatementContext* whereStatement();

  class  AndExprContext : public antlr4::ParserRuleContext {
  public:
    AndExprContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<SimpleExprContext *> simpleExpr();
    SimpleExprContext* simpleExpr(size_t i);
    std::vector<antlr4::tree::TerminalNode *> AND();
    antlr4::tree::TerminalNode* AND(size_t i);


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  AndExprContext* andExpr();

  class  SimpleExprContext : public antlr4::ParserRuleContext {
  public:
    SimpleExprContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    ComparisonExprContext *comparisonExpr();
    LikeExprContext *likeExpr();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  SimpleExprContext* simpleExpr();

  class  ComparisonExprContext : public antlr4::ParserRuleContext {
  public:
    antlr4::Token *op = nullptr;
    ComparisonExprContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<ColumnOrConstantContext *> columnOrConstant();
    ColumnOrConstantContext* columnOrConstant(size_t i);
    antlr4::tree::TerminalNode *EQ();
    antlr4::tree::TerminalNode *NE();
    antlr4::tree::TerminalNode *LT();
    antlr4::tree::TerminalNode *GT();
    antlr4::tree::TerminalNode *LE();
    antlr4::tree::TerminalNode *GE();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  ComparisonExprContext* comparisonExpr();

  class  LikeExprContext : public antlr4::ParserRuleContext {
  public:
    LikeExprContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    ColumnContext *column();
    antlr4::tree::TerminalNode *LIKE();
    antlr4::tree::TerminalNode *STRING();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  LikeExprContext* likeExpr();

  class  ColumnOrConstantContext : public antlr4::ParserRuleContext {
  public:
    ColumnOrConstantContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    ColumnContext *column();
    ConstantContext *constant();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  ColumnOrConstantContext* columnOrConstant();

  class  ColumnContext : public antlr4::ParserRuleContext {
  public:
    ColumnContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<IdentifierContext *> identifier();
    IdentifierContext* identifier(size_t i);
    antlr4::tree::TerminalNode *DOT();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  ColumnContext* column();

  class  ConstantContext : public antlr4::ParserRuleContext {
  public:
    ConstantContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *INTEGER();
    antlr4::tree::TerminalNode *STRING();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  ConstantContext* constant();

  class  IdentifierContext : public antlr4::ParserRuleContext {
  public:
    IdentifierContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *IDENTIFIER();
    KeywordContext *keyword();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  IdentifierContext* identifier();

  class  LimitStatementContext : public antlr4::ParserRuleContext {
  public:
    LimitStatementContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *LIMIT();
    antlr4::tree::TerminalNode *INTEGER();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  LimitStatementContext* limitStatement();

  class  KeywordContext : public antlr4::ParserRuleContext {
  public:
    KeywordContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *AND();
    antlr4::tree::TerminalNode *AS();
    antlr4::tree::TerminalNode *CREATE();
    antlr4::tree::TerminalNode *DISTINCT();
    antlr4::tree::TerminalNode *FROM();
    antlr4::tree::TerminalNode *INSERT();
    antlr4::tree::TerminalNode *INTO();
    antlr4::tree::TerminalNode *INT();
    antlr4::tree::TerminalNode *LIKE();
    antlr4::tree::TerminalNode *LIMIT();
    antlr4::tree::TerminalNode *OR();
    antlr4::tree::TerminalNode *SELECT();
    antlr4::tree::TerminalNode *STR();
    antlr4::tree::TerminalNode *TABLE();
    antlr4::tree::TerminalNode *VALUES();
    antlr4::tree::TerminalNode *WHERE();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  KeywordContext* keyword();


  // By default the static state used to implement the parser is lazily initialized during the first
  // call to the constructor. You can call this function if you wish to initialize the static state
  // ahead of time.
  static void initialize();

private:
};

