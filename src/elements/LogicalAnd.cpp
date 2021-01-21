//
// Created by chris on 21.01.21.
//

#include "LogicalAnd.h"

void LogicalAnd::createConditionBranch(TransformContext& context, llvm::BasicBlock* activeBlock, llvm::BasicBlock* trueBlock, llvm::BasicBlock* falseBlock) {
    auto extraBB = context.createBasicBlock("logic-and-temp");
    left->createConditionBranch(context,activeBlock, extraBB, falseBlock);
    right->createConditionBranch(context, extraBB, trueBlock, falseBlock);
}

llvm::Value* LogicalAnd::createLeftValue(TransformContext &context) {

}

llvm::Value* LogicalAnd::createRightValue(TransformContext &context){

}