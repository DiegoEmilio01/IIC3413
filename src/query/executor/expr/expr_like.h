#pragma once

#include <regex>

#include "query/executor/expr/expr.h"
#include "query/parser/logical_plan/logical_plan.h"

class ExprLike : public Expr {
public:
    ExprLike(std::unique_ptr<Expr> child, std::string&& _pattern) :
        child   (std::move(child)),
        pattern (std::move(_pattern)),
        res     ((int64_t)0)
    {
        std::string regex_str;
        for (char c : pattern) {
            switch (c) {
                case '%': {
                    regex_str += ".+";
                    break;
                }
                case '_': {
                    regex_str += ".";
                    break;
                }
                default: {
                    regex_str += "[";
                    regex_str += c;
                    regex_str += "]";
                    break;
                }
            }
        }

        txt_regex = std::regex(regex_str);
    }

    const Value& eval() override {
        auto& child_eval = child->eval();

        bool like_matches = std::regex_match(child_eval.value.as_str, txt_regex);

        res.value.as_int = static_cast<int64_t>(like_matches);
        return res;
    }

    std::ostream& print_to_ostream(std::ostream& os) const override {
        os << *child << " LIKE \"" << pattern << '"';
        return os;
    }

private:
    std::unique_ptr<Expr> child;

    std::string pattern;

    std::regex txt_regex;

    Value res;
};