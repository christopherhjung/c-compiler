//
// Created by chris on 21.01.21.
//

#pragma once

#include "Binary.h"

class PrettyPrinter;

class LogicalOr : public Binary {
public:
    void createConditionBranch(TransformContext &context, llvm::BasicBlock *activeBlock, llvm::BasicBlock *trueBlock,
                               llvm::BasicBlock *falseBlock) override;

    llvm::Value *createLeftValue(TransformContext &context) override;

    llvm::Value *createRightValue(TransformContext &context) override;
};



