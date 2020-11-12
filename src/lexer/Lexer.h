//
// Created by Christopher Jung on 2020-11-06.
//

#pragma once


#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include "../reader/InputReader.h"
#include "StateMachineBuilder.h"
#include "Token.h"
#include "Error.h"


class Lexer {

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

    bool hasNextToken(){
        reader->reset();

        if(!reader->hasNext()){
            return false;
        }

        int32_t acceptPosition = -1;
        State* currentState = state.get();
        State* acceptState = nullptr;
        Location location(reader->getOrigin(),line,column);
        while(true){
            if(currentState == nullptr){
                if(acceptPosition == -1){
                    errorObj = Error(location, reader->readString(reader->getPosition()) );
                    error = true;
                    return false;
                }
                break;
            }else if(currentState->finish){
                acceptPosition = reader->getPosition();
                acceptState = currentState;
            }

            if(!reader->hasNext()){
                if(acceptPosition == -1){
                    errorObj = Error(location, reader->readString(reader->getPosition()));
                    error = true;
                    return false;
                }else{
                    break;
                }
            }

            char c = reader->peek();
            updatePosition(c);
            reader->next();
            currentState = currentState->transitions[c];
        }

        token = Token(location,acceptState->id,acceptState->name, reader->readString(acceptPosition));
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

    Token fetchToken(){
        return token;
    }

    Error getError(){
        return errorObj;
    }

    bool isError(){
        return error;
    }
private:
    int line = 1;
    int column = 1;
    bool error = false;
    Error errorObj = {Location("",0,0),""};
    Token token = {Location("",0,0),0,"",""};
    std::unique_ptr<State> state;
    InputReader* reader;
};