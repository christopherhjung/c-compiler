//
// Created by chris on 21.01.21.
//

#include "LogicalOr.h"

void LogicalOr::createConditionBranch(TransformContext& context, llvm::BasicBlock* activeBlock, llvm::BasicBlock* trueBlock, llvm::BasicBlock* falseBlock)  {
    auto extraBB = context.createBasicBlock("logic-and-temp");
    left->createConditionBranch(context,activeBlock, trueBlock, extraBB);
    right->createConditionBranch(context, extraBB, trueBlock, falseBlock);
}

llvm::Value* LogicalOr::createLeftValue(TransformContext &context){

}

llvm::Value* LogicalOr::createRightValue(TransformContext &context){

}