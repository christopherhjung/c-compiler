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

    static unsigned int getSize(TransformContext &context, const SemanticType* semanticType);
    static llvm::Constant* getInt32Size(TransformContext &context, const SemanticType* semanticType);
    static llvm::Constant* getInt64Size(TransformContext &context, const SemanticType* semanticType);

    void dump(PrettyPrinter &printer) override;
};


