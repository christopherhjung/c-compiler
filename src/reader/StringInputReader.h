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
    uint32_t offset = 0;
    int16_t current;

    void assign() {
        if (index + offset >= getSize()) {
            current = 256;
        } else {
            current = str[index + offset];
        }
    }

public:
    ~StringInputReader() override {

    }

public:
    StringInputReader() {

    }

    StringInputReader(const std::string &str) {
        reset(str);
    }

    void reset(const std::string &str) {
        this->str = str;
        index = 0;
        offset = 0;
        assign();
    };

    int16_t peek() override {
        return current;
    }

    int16_t next() override {
        offset++;
        assign();
        return current;
    }

    uint32_t getSize() override {
        return str.length();
    }

    int16_t get(uint32_t index) override {
        return str[this->index + index];
    }

    void reset(uint32_t offset) override {
        this->index += offset;
        this->offset = 0;
        assign();
    }

    uint32_t getOffset() override {
        return offset;
    }

    std::string readString(uint32_t count) override {
        char arr[count];
        const char *src = str.c_str();
        std::copy(src + index, src + index + count, arr);
        return std::string(arr, count);
    }

    bool readLine(std::string &str) override {
        str = "";
        return false;
    }

    std::string getContext() override {
        return "";
    }
};


