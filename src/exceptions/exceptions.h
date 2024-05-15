#pragma once

#include <exception>
#include <string>

class IIC3413DBException : public std::exception {
public:
    std::string msg;

    explicit IIC3413DBException(const std::string& msg) : msg(msg) { }

    const char* what() const noexcept override {
        return msg.c_str();
    }
};


// Exceptions related to the query syntax
class QueryParsingException : public IIC3413DBException {
public:
    explicit QueryParsingException(const std::string& msg) : IIC3413DBException(msg) { }
};

// Exceptions related to the query semantic
class QueryException : public IIC3413DBException {
public:
    explicit QueryException(const std::string& msg) : IIC3413DBException(msg) { }
};

// Things that should not be exceptions at some point in the future
class NotImplementedException : public IIC3413DBException {
public:
    explicit NotImplementedException(const std::string& msg) : IIC3413DBException(msg) { }
};