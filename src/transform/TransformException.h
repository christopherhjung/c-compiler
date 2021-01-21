//
// Created by chris on 20.01.21.
//


#pragma once

#include <exception>
#include <functional>

class TransformException : public std::exception {
public:
    std::string msg = "";
    std::string file = "";
    int32_t lineNumber = 0;

    explicit TransformException(const std::string &file, int32_t lineNumber) : file(file), lineNumber(lineNumber) {

    }

    explicit TransformException(const std::string &msg, const std::string &file, int32_t lineNumber) : file(file),
                                                                                                       lineNumber(
                                                                                                               lineNumber),
                                                                                                       msg(msg) {

    }
};

#define TRANSFORM_ERROR() (throw TransformException(__FILE__, __LINE__))
#define TRANSFORM_ERROR_MSG(MSG) (throw TransformException(MSG,__FILE__, __LINE__))

