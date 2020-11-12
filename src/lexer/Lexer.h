//
// Created by Christopher Jung on 2020-11-06.
//

#pragma once


#include <iostream>
#include <fstream>
#include <memory>
#include <string>
#include <sstream>

#include "../reader/InputReader.h"
#include "StateMachineBuilder.h"
#include "Token.h"
#include "Error.h"
#import "SymbolCache.h"


class Lexer {
private:
    uint32_t line = 1;
    uint32_t column = 1;
    bool error = false;
    std::shared_ptr<Error> errorObj;
    std::shared_ptr<Token> token;
    std::unique_ptr<State> state;
    InputReader* reader;
    SymbolCache symbolCache;
public:
    Lexer(const std::string& file) : state(readStateMachine(file)){

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

    void reset(InputReader* reader){
        this->reader = reader;
        this->reader->mark();
    }

    std::string escaping(char cha){
        switch(cha){
            case '\n': return "\\n";
            case '\t': return "\\t";
            case '\r': return "\\r";
            default: return std::string(cha, 1);
        }
    }

    bool hasNextToken(){
        reader->reset();

        if(!reader->hasCurrent()){
            return false;
        }

        int32_t acceptPosition = -1;
        State* currentState = state.get();
        State* acceptState = nullptr;
        auto location = std::make_unique<Location>(reader->getOrigin(),line,column);
        char c = -1;
        while(true){
            if(currentState == nullptr || !reader->hasCurrent()){
                if(acceptPosition == -1){
                    errorObj = std::make_shared<Error>(location, reader->readString(reader->getPosition() - 1) + "_<-- char \"" + escaping(c) + "\" wrong!" );
                    error = true;
                    return false;
                }
                break;
            }

            c = reader->peek();
            reader->next();
            currentState = currentState->transitions[c];

            if(currentState != nullptr && currentState->finish){
                acceptPosition = reader->getPosition();
                acceptState = currentState;
            }
        }

        std::shared_ptr<std::string> value = symbolCache.internalize(reader->readString(acceptPosition));

        for(auto& c : *value){
            updatePosition(c);
        }

        token = std::make_shared<Token>(location,acceptState->id,acceptState->name, value );
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

    std::shared_ptr<Token> fetchToken(){
        return token;
    }

    std::shared_ptr<Error> getError(){
        return errorObj;
    }

    bool isError(){
        return error;
    }
};