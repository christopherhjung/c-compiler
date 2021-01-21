//
// Created by chris on 21.01.21.
//

#pragma once

#include "Elements.h"

class Unary : public Expression{
public:
    Expression* value = nullptr;
    Operator* op = nullptr;

    void dump(PrettyPrinter& printer) override{
        printer << "(";
        op->dump(printer);
        value->dump(printer);
        printer << ")";
    }
};



