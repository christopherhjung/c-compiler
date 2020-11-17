//
// Created by Christopher Jung on 09.11.20.
//

#pragma once

#include <string>

class InputReader {
public:
    virtual char peek() = 0;
    virtual char next() = 0;
    virtual void setMarker(uint32_t index) = 0;
    virtual uint32_t getOffset() = 0;
    virtual uint32_t getSize() = 0;
    virtual void reset() = 0;
    virtual std::string readString(uint32_t count) = 0;
};

