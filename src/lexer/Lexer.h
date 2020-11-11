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
        this->reader->next();
        finish = false;
    }

    void triggerError(const Location& location){
        errorObj = Error(location, std::string(ss.str()) + " <-----" );
        error = true;
    }

    bool hasNextToken(){
        finish = false;
        currentState = state.get();
        ss.str(std::string());

        if(!reader->hasNext()){
            return false;
        }

        Location location(reader->getOrigin(),line,column);
        while(true){
            if(currentState == nullptr){
                if(!finish){
                    triggerError(location);
                    return false;
                }
                break;
            }else if(currentState->finish){
                finish = true;
            }else if(finish){
                break;
            }

            if(!reader->hasNext()){
                if(!finish){
                    triggerError(location);
                    return false;
                }else{
                    break;
                }
            }

            char c = reader->peek();

            if(currentState->transitions.find(c) == currentState->transitions.end()){
                if(finish){
                    break;
                }else{
                    triggerError(location);
                    return false;
                }
            }

            updatePosition(c);
            reader->next();

            ss << c;
            currentState = currentState->transitions[c];
        }

        token = Token(location,currentState->id,currentState->name, new std::string(ss.str()));

        return finish;
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
    bool finish = false;
    bool error = false;
    Error errorObj = {Location("",0,0),""};
    Token token = {Location("",0,0),0,"",new std::string()};
    std::unique_ptr<State> state;
    State* currentState;
    InputReader* reader;
    std::stringstream ss;
};