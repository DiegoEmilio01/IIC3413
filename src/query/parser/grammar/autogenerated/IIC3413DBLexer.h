
// Generated from IIC3413DBLexer.g4 by ANTLR 4.13.1

#pragma once


#include "antlr4-runtime.h"




class  IIC3413DBLexer : public antlr4::Lexer {
public:
  enum {
    AND = 1, AS = 2, BETWEEN = 3, CREATE = 4, DISTINCT = 5, FROM = 6, INSERT = 7, 
    INTO = 8, INT = 9, LIKE = 10, LIMIT = 11, OR = 12, SELECT = 13, STR = 14, 
    TABLE = 15, VALUES = 16, WHERE = 17, EQ = 18, NE = 19, GT = 20, LT = 21, 
    GE = 22, LE = 23, COMMA = 24, ASTERISK = 25, MINUS = 26, SLASH = 27, 
    PLUS = 28, DOT = 29, L_PAR = 30, R_PAR = 31, INTEGER = 32, STRING = 33, 
    IDENTIFIER = 34, WHITE_SPACE = 35, COMMENT_INPUT = 36, LINE_COMMENT = 37, 
    UNRECOGNIZED = 38
  };

  enum {
    WS_CHANNEL = 2
  };

  explicit IIC3413DBLexer(antlr4::CharStream *input);

  ~IIC3413DBLexer() override;


  std::string getGrammarFileName() const override;

  const std::vector<std::string>& getRuleNames() const override;

  const std::vector<std::string>& getChannelNames() const override;

  const std::vector<std::string>& getModeNames() const override;

  const antlr4::dfa::Vocabulary& getVocabulary() const override;

  antlr4::atn::SerializedATNView getSerializedATN() const override;

  const antlr4::atn::ATN& getATN() const override;

  // By default the static state used to implement the lexer is lazily initialized during the first
  // call to the constructor. You can call this function if you wish to initialize the static state
  // ahead of time.
  static void initialize();

private:

  // Individual action functions triggered by action() above.

  // Individual semantic predicate functions triggered by sempred() above.

};

