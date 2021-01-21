#pragma once

#include <unordered_set>
#include "Lexer.h"
#include "LexerControl.h"

class CatchingLexerProxy : public Lexer {

public:
    uint32_t line = 1;
    uint32_t column = 1;
    bool error = false;
    Error *errorObj;
    char last = 0;
    bool finish = false;
    LexerControl &control;

    CatchingLexerProxy(LexerControl &control) : control(control) {

    }

    bool hasNextToken(Token &token) override;

    void updatePosition(char c);

    void reset(InputReader *reader) override;

    Error *getError() override;

    bool isError() override;
};