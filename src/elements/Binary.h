//
// Created by chris on 21.01.21.
//

#pragma once

#include "Elements.h"
#include "../parser/PrettyPrinter.h"

class Binary : public Expression{
public:
    Expression* left = nullptr;
    Expression* right = nullptr;
    Operator* op = nullptr;

    void dump(PrettyPrinter& printer) override;
};


