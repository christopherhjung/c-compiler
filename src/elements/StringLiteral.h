//
// Created by chris on 21.01.21.
//

#pragma once

#include <string>
#include "Elements.h"

class StringLiteral : public Expression{
public:
    const std::string *value = nullptr;

    void dump(PrettyPrinter& printer) override{
        printer << *value;
    }
};



