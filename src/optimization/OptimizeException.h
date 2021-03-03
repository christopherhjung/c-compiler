//
// Created by chris on 20.01.21.
//


#pragma once

#include <exception>
#include <functional>
#include <string>

class OptimizeException : public std::exception {
public:
    std::string file;
    int32_t lineNumber;
    std::string msg;

    explicit OptimizeException(const std::string &file, int32_t lineNumber) : file(file), lineNumber(lineNumber), msg("") {

    }

    explicit OptimizeException(const std::string &msg, const std::string &file, int32_t lineNumber) : file(file),
                                                                                                       lineNumber(
                                                                                                               lineNumber),
                                                                                                       msg(msg) {

    }
};

#define OPTIMIZE_ERROR() (throw OptimizeException(__FILE__, __LINE__))
#define OPTIMIZE_ERROR_MSG(MSG) (throw OptimizeException(MSG,__FILE__, __LINE__))

