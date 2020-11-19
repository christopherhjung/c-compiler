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
    uint32_t index;
    uint32_t marker = 0;
public:
    void reset(const std::string& str){
        this->str = str;
        index = 0;
        marker = 0;
    };

    int16_t peek() override{
        return str[index + marker];
    }

    int16_t next() override{
        if(index + marker >= getSize()){
            return 256;
        }else{
            return str[index + ++marker];
        }
    }

    uint32_t getSize() override {
        return str.length();
    }

    void reset(uint32_t index) override {
        marker = 0;
        this->index += index;
    }

    uint32_t getOffset() override {
        return marker;
    }

    std::string readString(uint32_t count) override {
        char arr[count];
        const char* src = str.c_str();
        std::copy(src + index, src + index + count, arr);
        return std::string(arr, count);
    }
};


