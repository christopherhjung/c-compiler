//
// Created by chris on 21.01.21.
//

#pragma once

#include "Binary.h"

class PrettyPrinter;
class Identifier;

class Assignment : public Binary {
public:
    static llvm::Value *ensureAssignment(TransformContext &context, const SemanticType* leftType, Expression* right);
    static llvm::Value *makeAssignment(TransformContext &context, Identifier* left, Expression* right);
    static llvm::Value *makeAssignment(TransformContext &context, Expression* left, Expression* right);
    llvm::Value *createRightValue(TransformContext &context) override;
};


