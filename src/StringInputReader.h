//
// Created by Christopher Jung on 2020-11-06.
//

#ifndef C4_STRINGINPUTREADER_H
#define C4_STRINGINPUTREADER_H


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

    virtual char peek(){
        return str[index];
    }

    virtual void next(){
        index++;
    }

    virtual bool hasNext(){
        return index < str.length();
    }
};


#endif //C4_STRINGINPUTREADER_H
