//
// Created by chris on 21.01.21.
//

#pragma once

#include "Expression.h"

class PrettyPrinter;

class Operator;

class Unary : public Expression {
public:
    Expression *value = nullptr;
    Operator *op = nullptr;

    void dump(PrettyPrinter &printer) override;
};



