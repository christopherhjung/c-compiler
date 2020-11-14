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
#import "SymbolCache.h"
#include "Lexer.h"


class StateMachineLexer : public Lexer{
private:
    uint32_t line = 1;
    uint32_t column = 1;
    bool error = false;
    Error* errorObj;
    std::unique_ptr<State> state;
    SymbolCache symbolCache;
public:
    StateMachineLexer(const std::string& file) : state(readStateMachine(file)){

    };



    static std::unique_ptr<State> readStateMachine(const std::string& file){
        std::ifstream source (file);

        std::unique_ptr<State> state;
        StateMachineBuilder builder;
        if (source.is_open())
        {
            for( std::string line; getline( source, line ); )
            {
                int position = line.find(':');
                std::string key = line.substr(0, position);
                std::string value = line.substr(position + 1);

                builder.add(key, value);
            }

            state = builder.build();

            source.close();
        }
        return state;
    }

    std::string escaping(char cha){
        switch(cha){
            case '\n': return "\\n";
            case '\t': return "\\t";
            case '\r': return "\\r";
            default: return std::string(1,cha);
        }
    }

    bool hasNextToken(Token& token) override {
        reader->reset();

        if(!reader->hasCurrent()){
            return false;
        }

        int32_t acceptPosition = -1;
        State* currentState = state.get();
        State* acceptState = nullptr;
        token.location.line = line;
        token.location.column = column;
        char c = -1;
        while(true){
            if(currentState == nullptr || !reader->hasCurrent()){
                if(acceptPosition == -1){
                    errorObj =new Error(&token.location, reader->readString(reader->getOffset() - 1) + "_<-- char >" + escaping(c) + "< wrong!" );
                    error = true;
                    return false;
                }
                break;
            }

            c = reader->peek();
            reader->next();
            currentState = currentState->transitions[c];

            if(currentState != nullptr && currentState->finish){
                acceptPosition = reader->getOffset();
                acceptState = currentState;
            }
        }

        std::string value = symbolCache.internalize(reader->readString(acceptPosition));

        for(auto& c : value){
            updatePosition(c);
        }

        token.id = acceptState->id;
        token.name = acceptState->name;
        token.value = value;
        reader->setMarker(acceptPosition);
        return true;
    }

    void updatePosition(char c){
        if(c == '\n'){
            line++;
            column=1;
        }else{
            column++;
        }
    }

    Token* fetchToken() override {
        return nullptr;
    }

    Error* getError() override {
        return errorObj;
    }

    bool isError() override {
        return error;
    }
};