//
// Created by Christopher Jung on 09.11.20.
//

#pragma once

#include <string>

class InputReader {
public:
    virtual ~InputReader() = default;

public:
    virtual int16_t peek() = 0;
    virtual int16_t next() = 0;
    virtual void reset(uint32_t index) = 0;
    virtual uint32_t getOffset() = 0;
    virtual uint32_t getSize() = 0;
    virtual std::string readString(uint32_t count) = 0;
    virtual bool readLine(std::string& str) = 0;
    virtual std::string getContext() = 0;
    virtual int16_t get(uint32_t index) = 0;
};

