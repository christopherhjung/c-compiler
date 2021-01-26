//
// Created by chris on 21.01.21.
//

#include "IfSmall.h"
#include "../parser/PrettyPrinter.h"
#include "../transform/TransformContext.h"

void IfSmall::dump(PrettyPrinter &printer) {
    printer << "(";
    condition->dump(printer);
    printer << " ? ";
    left->dump(printer);
    printer << " : ";
    right->dump(printer);
    printer << ")";
}

llvm::Value *IfSmall::createRightValue(TransformContext &context) {
    auto trueBlock = context.createBasicBlock("write-left");
    auto falseBlock = context.createBasicBlock("write-right");
    auto end = context.createBasicBlock("end");
    condition->createConditionBranch(context, trueBlock, falseBlock);

    auto value = context.resetAllocBuilder().CreateAlloca(context.getType(left->superType));

    context.setCurrentBlock(trueBlock);
    context.builder.CreateStore(left->createRightValue(context), value);
    context.builder.CreateBr(end);

    context.setCurrentBlock(falseBlock);
    context.builder.CreateStore(right->createRightValue(context), value);
    context.builder.CreateBr(end);

    context.setCurrentBlock(end);
    return context.builder.CreateLoad(value);
}