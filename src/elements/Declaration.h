//
// Created by chris on 21.01.21.
//

#pragma once

#include "Statement.h"
#include "Expression.h"

class PrettyPrinter;

class Declarator;

class Type;

class Declaration : public Statement {
public:
    Type *type = nullptr;
    Declarator *declarator = nullptr;
    Expression* initializer = nullptr;
    Location assignLocation;

    void dump(PrettyPrinter &printer) override;

    void create(TransformContext &context) override;
};



