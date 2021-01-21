//
// Created by chris on 21.01.21.
//

#pragma once

#include "../transform/TransformContext.h"

#include "Elements.h"

class CompareBinary : public Binary{
public:
    void createConditionBranch(TransformContext& context, llvm::BasicBlock* activeBlock, llvm::BasicBlock* trueBlock, llvm::BasicBlock* falseBlock) override;

    llvm::CmpInst::Predicate getPredicate();

    llvm::Value* createRightValue(TransformContext &context) override;
};



