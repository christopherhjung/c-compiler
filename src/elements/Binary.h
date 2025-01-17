//
// Created by chris on 21.01.21.
//

#pragma once

#include "Expression.h"

class PrettyPrinter;

class Operator;

class Binary : public Expression {
public:
    Expression *left = nullptr;
    Expression *right = nullptr;
    Operator *op = nullptr;
    int opInfo = 0;

    void dump(PrettyPrinter &printer) override;

    llvm::Value *createRightValue(TransformContext &context) override;

    llvm::Value *createLeftValue(TransformContext &context) override;
};


