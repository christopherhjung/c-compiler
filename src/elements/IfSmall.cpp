//
// Created by chris on 21.01.21.
//

#include "IfSmall.h"
#include "../parser/PrettyPrinter.h"
#include "../transform/TransformContext.h"
#include "Assignment.h"

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

    context.setCurrentBlock(trueBlock);
    Assignment::makeAssignment(context, left, left->createRightValue(context))
    context.builder.CreateBr(end);

    context.setCurrentBlock(falseBlock);
    Assignment::makeAssignment(context, left, right->createRightValue(context))
    context.builder.CreateBr(end);

    context.setCurrentBlock(end);
    return context.builder.CreateLoad(value);
}