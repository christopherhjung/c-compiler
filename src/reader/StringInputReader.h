//
// Created by Christopher Jung on 2020-11-06.
//

#pragma once



#include <iostream>
#include <fstream>
#include <string>

#include "InputReader.h"



class StringInputReader : public InputReader {
    std::string str;
    int index;
public:
    void reset(const std::string& str){
        this->str = str;
        index = 0;
    };

    char peek() override{
        return str[index];
    }

    void next() override{
        index++;
    }

    bool hasNext() override{
        return index < str.length();
    }
};


