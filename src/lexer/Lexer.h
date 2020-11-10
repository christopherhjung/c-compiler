//
// Created by Christopher Jung on 2020-11-06.
//

#ifndef C4_LEXER_H
#define C4_LEXER_H

#include <iostream>
#include <fstream>
#include <string>

#include <sstream>

#include "../reader/InputReader.h"
#include "StateMachineBuilder.h"
#include "Token.h"



class Lexer {

public:
    Lexer(const std::string& file) : state(readStateMachine(file)){

    };

    static State* readStateMachine(const std::string& file){
        std::ifstream source (file);

        State* state = nullptr;
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

    bool hasNextToken(){
        finish = false;
        currentState = state;
        ss.str(std::string());
        int currentLine = line;
        int currentColumn = column;
        while(reader->hasNext()){
            if(currentState == nullptr){
                break;
            }else if(currentState->finish){
                finish = true;
            }else if(finish){
                break;
            }

            char c = reader->peek();

            if(currentState->transitions.find(c) == currentState->transitions.end()){
                break;
            }

            updatePosition(c);
            reader->next();

            ss << c;
            currentState = currentState->transitions[c];
        }

        token = Token(currentLine,currentColumn,currentState->id,new std::string(ss.str()));

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
private:
    int line = 1;
    int column = 1;
    bool finish = false;
    Token token = {0,0,0,new std::string()};
    State* state;
    State* currentState;
    InputReader* reader;
    std::stringstream ss;
};


#endif //C4_LEXER_H
