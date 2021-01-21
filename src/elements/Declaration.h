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

    llvm::Value *create(TransformContext &context) override;

    llvm::BasicBlock *create(TransformContext &context, llvm::BasicBlock *start) override;
};



