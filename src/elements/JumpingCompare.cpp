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






    context.setCurrentBlock(trueBlock);
    context.builder.CreateBr(end);

    context.setCurrentBlock(falseBlock);
    context.builder.CreateBr(end);


    context.setCurrentBlock(end);
    auto value =  context.builder.CreatePHI(context.builder.getInt32Ty(),2);
    value->addIncoming(context.builder.getInt32(1), trueBlock);
    value->addIncoming(context.builder.getInt32(0), falseBlock);
    return value;//context.builder.CreateLoad(value);
}