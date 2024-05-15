#pragma once

#include "antlr4-runtime.h"
#include "exceptions/exceptions.h"

namespace Parser {

class AntlrErrorListener : public antlr4::BaseErrorListener {
public:
    void syntaxError(antlr4::Recognizer* /*recognizer*/,
                     antlr4::Token*      /*offendingSymbol*/,
                     size_t              line,
                     size_t              char_pos_in_line,
                     const std::string&  msg,
                     std::exception_ptr  /*e*/) override
    {
        throw QueryParsingException("Line " + std::to_string(line) + ":" + std::to_string(char_pos_in_line) + " " + msg);
    }
};
} // namespace Parser