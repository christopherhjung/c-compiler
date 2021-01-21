//
// Created by chris on 21.01.21.
//

#pragma once

#include "Expression.h"

class PrettyPrinter;

class Select : public Expression {
public:
    Expression *target = nullptr;
    Expression *index = nullptr;

    void dump(PrettyPrinter &printer) override;
};


