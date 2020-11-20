//
// Created by Christopher Jung on 2020-11-06.
//

#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <memory>

#include <unordered_set>
#include <unordered_map>
#include <utility>
#include <vector>
#include "../reader/StringInputReader.h"

struct Info{

};

class ParserCodeGenerator {
public:
    StringInputReader reader;

    char eat(){
        char result = reader.peek();
        if(hasNext()){
            reader.next();
        }
        return result;
    }

    bool eat(char cha){
        if(reader.peek() == cha){
            reader.next();
            return true;
        }
        return false;
    }

    bool is(char cha){
        return reader.peek() == cha;
    }

    bool hasNext(){
        return reader.peek() != 0;
    }

    void add(const std::string& name, const std::string& regex, bool greedy)
    {
        parseOr();
    }

    Info* parseOr()
    {
        Info* left = parseConcat();

        if ( eat('|'))
        {

        }

        return left;
    }

    Info* parseConcat()
    {
        Info* leftState = parseMultiplier();

        if(eat(' '))
        {

        }

        return leftState;
    }

    Info* parseMultiplier()
    {
        Info* state = parseParenthesis();

        if (hasNext())
        {
            uint8_t type;
            if (eat('*'))
            {
                type = 1;
            }
            else if (eat('?'))
            {
                type = 0;
            }
            else if (eat('+'))
            {
                type = 2;
            }else{
                return state;
            }


        }

        return state;
    }

    Info* parseParenthesis()
    {
        if (hasNext())
        {
            if (eat('('))
            {
                Info* state = parseOr();
                if (!eat(')'))
                {
                    throw std::exception();
                }

                return state;
            }
        }

        return nullptr;
    }

};
