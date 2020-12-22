//
// Created by Christopher Jung on 22.12.20.
//

#pragma once


class SemanticException : public std::exception{
public:
    const Location location;
    int32_t  lineNumber;

    explicit SemanticException(const Location& location) : location(location){

    }
    explicit SemanticException(const Location& location, int32_t lineNumber) : location(location), lineNumber(lineNumber){

    }
};