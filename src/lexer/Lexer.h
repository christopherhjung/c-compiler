//
// Created by Christopher Jung on 13.11.20.
//

#pragma once

#include "Token.h"
#include "Error.h"
#include "Lexer.h"
#include "../reader/InputReader.h"

class Lexer {
public:
    int16_t current;
    int32_t accept;
    uint32_t offset;
    InputReader *reader;

    virtual void reset(InputReader *reader) {
        this->reader = reader;
        this->reader->reset(0);
    }

    virtual bool hasNextToken(Token &token) = 0;

    virtual Error *getError() = 0;

    virtual bool isError() = 0;
};