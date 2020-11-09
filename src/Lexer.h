//
// Created by Christopher Jung on 2020-11-06.
//

#ifndef C4_LEXER_H
#define C4_LEXER_H

#include <iostream>
#include <fstream>
#include <string>

#include <sstream>

#include "InputReader.h"
#include "RegExParser.h"

using namespace std;

class Lexer {

public:
    Lexer(const string& file) : state(readStateMachine(file)){

    };

    static State* readStateMachine(const string& file){
        ifstream source (file);

        State* state = nullptr;
        RegExParser regExParser;
        if (source.is_open())
        {
            for( std::string line; getline( source, line ); )
            {
                int position = line.find(':');
                string key = line.substr(0, position);
                string value = line.substr(position + 1);

                regExParser.add(key, value);
            }

            state = regExParser.build();

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

            reader->next();

            ss << c;
            currentState = currentState->transitions[c];
        }

        ss << ":" << currentState->name;

        return finish;
    }

    string fetchToken(){
        return ss.str();
    }
private:
    bool finish = false;
    State* state;
    State* currentState;
    InputReader* reader;
    std::stringstream ss;
};


#endif //C4_LEXER_H
