//
// Created by chris on 21.01.21.
//

#pragma once

#include "Statement.h"

class PrettyPrinter;

class Declarator;

class Type;

class Declaration : public Statement {
public:
    Type *type = nullptr;
    Declarator *declarator = nullptr;

    void dump(PrettyPrinter &printer) override;

    void create(TransformContext &context) override;
};



