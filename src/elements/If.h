//
// Created by chris on 21.01.21.
//

#pragma once

#include "Elements.h"

class If : public Statement{
public:
    Expression* condition = nullptr;
    Statement* trueBranch = nullptr;
    bool hasFalse = false;
    Statement* falseBranch = nullptr;

    void dump(PrettyPrinter& printer) override;

    llvm::BasicBlock* create(TransformContext &context, llvm::BasicBlock* start) override;
};



