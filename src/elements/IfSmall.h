//
// Created by chris on 21.01.21.
//

#pragma once

#include "Expression.h"

class PrettyPrinter;

class IfSmall : public Expression {
public:
    Expression *condition = nullptr;
    Expression *left = nullptr;
    Expression *right = nullptr;

    void dump(PrettyPrinter &printer) override;
};



