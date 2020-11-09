//
// Created by Christopher Jung on 2020-11-06.
//

#ifndef C4_STRINGINPUTREADER_H
#define C4_STRINGINPUTREADER_H


#include <iostream>
#include <fstream>
#include <string>

#include "InputReader.h"

using namespace std;

class StringInputReader : public InputReader {
    string str;
    int index;
public:
    void reset(const string& str){
        this->str = str;
        index = 0;
    };

    char peek(){
        return str[index];
    }

    char next(){
        index++;
        return peek();
    }

    bool hasNext(){
        return index < str.length();
    }
};


#endif //C4_STRINGINPUTREADER_H
