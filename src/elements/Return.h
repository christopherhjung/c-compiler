//
// Created by chris on 21.01.21.
//

#pragma once

#include "Statement.h"

class PrettyPrinter;

class Expression;

class Return : public Statement {
public:
    Expression *value = nullptr;

    void dump(PrettyPrinter &printer) override;

    void create(TransformContext &context) override;
};


