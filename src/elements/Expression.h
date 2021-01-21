//
// Created by chris on 21.01.21.
//
#pragma once

#include "Statement.h"
class Expression : public Statement{
public:
    virtual llvm::Value* createRightValue(TransformContext &context);
    virtual llvm::Value* createLeftValue(TransformContext &context);
    virtual void createConditionBranch(TransformContext& context, llvm::BasicBlock* activeBlock, llvm::BasicBlock* trueBlock, llvm::BasicBlock* falseBlock);
};

