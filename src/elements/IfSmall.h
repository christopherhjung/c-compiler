//
// Created by chris on 21.01.21.
//

#pragma once

#include "Elements.h"

class IfSmall : public Expression{
public:
    Expression* condition = nullptr;
    Expression* left = nullptr;
    Expression* right = nullptr;

    void dump(PrettyPrinter& printer) override{
        printer << "(";
        condition->dump(printer);
        printer << " ? ";
        left->dump(printer);
        printer << " : ";
        right->dump(printer);
        printer << ")";
    }
};



