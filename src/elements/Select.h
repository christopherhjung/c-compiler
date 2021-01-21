//
// Created by chris on 21.01.21.
//

#pragma once

#include "Elements.h"

class Select : public Expression{
public:
    Expression* target = nullptr;
    Expression* index = nullptr;

    void dump(PrettyPrinter& printer) override{
        printer << "(";
        target->dump(printer);
        printer << "[";
        index->dump(printer);
        printer << "]";
        printer << ")";
    }
};


