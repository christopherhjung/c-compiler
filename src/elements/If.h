//
// Created by chris on 21.01.21.
//

#pragma once

#include "Statement.h"

class PrettyPrinter;

class Expression;

class If : public Statement {
public:
    Expression *condition = nullptr;
    Statement *trueBranch = nullptr;
    bool hasFalse = false;
    Statement *falseBranch = nullptr;

    void dump(PrettyPrinter &printer) override;

    void create(TransformContext &context) override;
};



