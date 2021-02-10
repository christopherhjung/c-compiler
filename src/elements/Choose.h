//
// Created by chris on 21.01.21.
//

#pragma once

#include "Expression.h"
#include "JumpingCompare.h"

class PrettyPrinter;

class Choose : public Expression {
public:
    Expression *condition = nullptr;
    Expression *left = nullptr;
    Expression *right = nullptr;

    void dump(PrettyPrinter &printer) override;

    llvm::Value *createRightValue(TransformContext &context) override;
};



