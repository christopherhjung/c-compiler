//
// Created by chris on 21.01.21.
//

#include "JumpingCompare.h"

#include "../transform/TransformContext.h"

llvm::Value *JumpingCompare::createRightValue(TransformContext &context) {
    auto trueBlock = context.createBasicBlock("write-1");
    auto falseBlock = context.createBasicBlock("write-0");
    auto end = context.createBasicBlock("end");
    createConditionBranch(context, trueBlock, falseBlock);

    auto value = context.resetAllocBuilder().CreateAlloca(context.builder.getInt32Ty());

    context.setCurrentBlock(trueBlock);
    context.builder.CreateStore(context.builder.getInt32(1), value);
    context.builder.CreateBr(end);

    context.setCurrentBlock(falseBlock);
    context.builder.CreateStore(context.builder.getInt32(0), value);
    context.builder.CreateBr(end);

    context.setCurrentBlock(end);
    return context.builder.CreateLoad(value);
}