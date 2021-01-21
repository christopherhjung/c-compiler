//
// Created by chris on 21.01.21.
//

#pragma once


#include "Elements.h"
#include <string>

class Constant : public Expression{
public:
    const std::string *value = nullptr;

    void dump(PrettyPrinter& printer) override{
        printer << *value;
    }
};



