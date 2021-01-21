#pragma once

#include "Lexer.h"
#include "LexerControl.h"

class LexerProxy : public Lexer {

public:
    uint32_t line = 1;
    uint32_t column = 1;
    bool error = false;
    Error *errorObj;
    char last = 0;
    bool finish = false;
    LexerControl &control;
    std::string value;

    LexerProxy(LexerControl &control) : control(control) {

    }

    bool hasNextToken(Token &token) override {
        int32_t startLine = line;
        int32_t startColumn = column;
        while (true) {
            if (current == 256) {
                if (finish) {
                    return false;
                } else {
                    accept = 0;
                    finish = true;
                    break;
                }
            }
            accept = -1;
            offset = 0;
            control.parse();
            if (accept == -1) {
                token.location.line = startLine;
                token.location.column = startColumn;
                std::string msg = reader->readString(reader->getOffset()) + "_<-- char >";
                if (current == 256) {
                    msg += "EOL";
                } else if (current == 0) {
                    msg += "NULL";
                } else {
                    msg += std::to_string((char) current);
                }
                msg += "< wrong!";
                errorObj = new Error(token.location, msg);
                error = true;
                return false;
            }

            for (uint32_t i = 0; i < offset; i++) {
                updatePosition(reader->get(i));
            }

            if (control.isHiding(accept)) {
                value = reader->readString(offset);
                token.value = &value;
                reader->reset(offset);
                current = reader->peek();
                break;
            } else {
                reader->reset(offset);
                current = reader->peek();
                startLine = line;
                startColumn = column;
            }
        }
        token.location.line = startLine;
        token.location.column = startColumn;
        token.id = accept;
        return true;
    }

    void updatePosition(char c) {
        if (c == '\r') {
            if (last == '\n') {
                last = 0;
            } else {
                column = 1;
                line++;
                last = c;
            }
        } else if (c == '\n') {
            if (last == '\r') {
                last = 0;
            } else {
                column = 1;
                line++;
                last = c;
            }
        } else {
            column++;
            last = c;
        }
    }

    void reset(InputReader *reader) override {
        Lexer::reset(reader);
        current = reader->peek();
        control.init(*this);
    }

    Error *getError() override {
        return errorObj;
    }

    bool isError() override {
        return error;
    }
};