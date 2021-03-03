//
// Created by chris on 21.01.21.
//

#include "Choose.h"
#include "../parser/PrettyPrinter.h"
#include "../transform/TransformContext.h"
#include "Assignment.h"

void Choose::dump(PrettyPrinter &printer) {
    printer << "(";
    condition->dump(printer);
    printer << " ? ";
    left->dump(printer);
    printer << " : ";
    right->dump(printer);
    printer << ")";
}

llvm::Value *Choose::createRightValue(TransformContext &context) {
    auto trueBlock = context.createBasicBlock("write-left");
    auto falseBlock = context.createBasicBlock("write-right");
    auto end = context.createBasicBlock("end");
    condition->createConditionBranch(context, trueBlock, falseBlock);

    auto targetType = left->getType()->packMethodType();
    auto value = context.resetAllocBuilder().CreateAlloca(context.getType(targetType));

    context.setCurrentBlock(trueBlock);
    //Assignment::makeAssignment(context, left, left->createRightValue(context))
    context.builder.CreateStore(left->createRightValue(context), value);
    context.builder.CreateBr(end);

    context.setCurrentBlock(falseBlock);
    //Assignment::makeAssignment(context, left, right->createRightValue(context))
    context.builder.CreateStore(right->createRightValue(context), value);
    context.builder.CreateBr(end);

    context.setCurrentBlock(end);
    return context.builder.CreateLoad(value);
}