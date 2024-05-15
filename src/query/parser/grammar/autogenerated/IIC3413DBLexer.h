
// Generated from IIC3413DBLexer.g4 by ANTLR 4.13.1

#pragma once


#include "antlr4-runtime.h"




class  IIC3413DBLexer : public antlr4::Lexer {
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

