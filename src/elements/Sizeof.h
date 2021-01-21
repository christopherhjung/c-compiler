//
// Created by chris on 21.01.21.
//

#pragma once

#include "Expression.h"

class PrettyPrinter;

class Type;

class Declarator;

class Sizeof : public Expression {
public:
    Type *type = nullptr;
    Declarator *declarator = nullptr;

    void dump(PrettyPrinter &printer) override;
};


