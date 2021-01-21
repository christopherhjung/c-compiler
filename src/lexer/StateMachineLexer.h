//
// Created by Christopher Jung on 20.11.20.
//

#pragma once


//
// Created by Christopher Jung on 2020-11-06.
//

#pragma once


#include <iostream>
#include <fstream>
#include <memory>
#include <string>
#include <sstream>
#include <functional>

#include "../reader/InputReader.h"
#include "StateMachineBuilder.h"
#include "Token.h"
#include "Error.h"
#include "SymbolCache.h"
#include "Lexer.h"


class StateMachineLexer : public Lexer {
private:
    uint32_t line = 1;
    uint32_t column = 1;
    bool error = false;
    Error *errorObj;
    std::shared_ptr<State> root;
    SymbolCache symbolCache;
    StateMachine *machine;
    bool finish;
public:
    StateMachineLexer(StateMachine *machine) : root(machine->root), machine(machine) {

    };

    std::string escaping(char cha) {
        switch (cha) {
            case '\n':
                return "\\n";
            case '\t':
                return "\\t";
            case '\r':
                return "\\r";
            default:
                return std::string(1, cha);
        }
    }

    bool hasNextToken(Token &token) override {
        int32_t acceptPosition;
        State *currentState;
        State *acceptState = nullptr;
        std::string value;
        while (true) {
            if (reader->peek() == 256) {
                if (finish) {
                    return false;
                } else {
                    token.id = machine->eof;
                    token.value = nullptr;
                    finish = true;
                    return true;
                }
            }

            acceptPosition = -1;
            currentState = root.get();
            acceptState = nullptr;
            token.location.line = line;
            token.location.column = column;
            char c = -1;
            while (true) {
                if (currentState == nullptr || reader->peek() == 256) {
                    if (acceptPosition == -1) {
                        token.id = -1;
                        errorObj = new Error(token.location,
                                             reader->readString(reader->getOffset() - 1) + "_<-- char >" + escaping(c) +
                                             "< wrong!");
                        error = true;
                        return false;
                    }
                    break;
                }

                c = reader->peek();
                reader->next();
                currentState = currentState->transitions[c];

                if (currentState != nullptr && currentState->finish) {
                    acceptPosition = reader->getOffset();
                    acceptState = currentState;
                }
            }

            token.value = &symbolCache.internalize(reader->readString(acceptPosition));

            for (auto &c2 : value) {
                updatePosition(c2);
            }

            reader->reset(acceptPosition);

            if (!machine->hides[acceptState->id]) {
                break;
            }
        }

        token.id = acceptState->id;
        return true;
    }

    void updatePosition(char c) {
        if (c == '\n') {
            line++;
            column = 1;
        } else {
            column++;
        }
    }

    Error *getError() override {
        return errorObj;
    }

    bool isError() override {
        return error;
    }
};

