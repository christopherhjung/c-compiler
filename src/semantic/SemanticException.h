//
// Created by Christopher Jung on 22.12.20.
//

#pragma once

#include <functional>
#include <exception>
#include "../lexer/Location.h"

class SemanticException : public std::exception {
public:
    const Location location;
    std::string msg = "";
    int32_t lineNumber = 0;

    explicit SemanticException(const Location &location) : location(location) {

    }

    explicit SemanticException(const Location &location, int32_t lineNumber) : location(location),
                                                                               lineNumber(lineNumber) {

    }

    explicit SemanticException(const Location &location, const std::string &msg, int32_t lineNumber) : location(
            location), lineNumber(lineNumber), msg(msg) {

    }
};

#define ERROR(LOCATION) (throw SemanticException(LOCATION, __LINE__))
#define ERROR_MSG(LOCATION, MSG) (throw SemanticException(LOCATION,MSG, __LINE__))
