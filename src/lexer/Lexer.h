//
// Created by Christopher Jung on 13.11.20.
//

#pragma once

#include "Token.h"
#include "Error.h"
#include "Lexer.h"
#include "../reader/InputReader.h"

class Lexer {
protected:
    InputReader* reader;
public:
    virtual void reset(InputReader* reader){
        this->reader = reader;
        this->reader->mark();
    }

    virtual bool hasNextToken(Token& token) = 0;

    virtual Token* fetchToken() = 0;

    virtual Error* getError() = 0;

    virtual bool isError() = 0;
};