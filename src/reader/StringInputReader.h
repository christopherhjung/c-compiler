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
    uint32_t marker = -1;
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

    void mark() override {
        marker = index;
    }

    void setMarker(uint32_t index) override {
        marker = index;
    }

    uint32_t getPosition() override {
        return index;
    }

    void reset() override {
        index = marker;
    }

    bool hasCurrent() override{
        return index < str.length();
    }

    std::string readString(uint32_t count) override {
        char arr[count];
        const char* src = str.c_str();
        std::copy(src, src + count, arr);
        return std::string(arr);
    }
};


