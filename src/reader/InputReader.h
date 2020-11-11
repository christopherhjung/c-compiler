//
// Created by Christopher Jung on 09.11.20.
//

#pragma once

#include <string>

class InputReader {
public:
    virtual char peek() = 0;
    virtual void next() = 0;
    virtual bool hasNext() = 0;
    virtual std::string getOrigin(){
        return "";
    };
};

