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
    SemanticType *inner = nullptr;
    Declarator *declarator = nullptr;

    llvm::Value *createRightValue(TransformContext &context) override;

    static llvm::Value* getSize(TransformContext &context, const SemanticType* type);

    void dump(PrettyPrinter &printer) override;
};


