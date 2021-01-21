//
// Created by chris on 21.01.21.
//

#include "If.h"

#include "Util.h"
#include "../parser/PrettyPrinter.h"
#include "Block.h"
#include "Expression.h"
#include "../transform/TransformContext.h"

void If::dump(PrettyPrinter &printer) {
    printer << "if (";
    condition->dump(printer);
    printer << ")";

    printIndentIfNotBlock(printer, trueBranch);

    if (hasFalse) {
        if (instanceof<Block>(trueBranch)) {
            printer << " ";
        } else {
            printer.newLine();
        }

        printer << "else";

        if (instanceof<If>(falseBranch)) {
            printer << " ";
            falseBranch->dump(printer);
        } else {
            printIndentIfNotBlock(printer, falseBranch);
        }
    }
}

llvm::BasicBlock *If::create(TransformContext &context, llvm::BasicBlock *start) {
    llvm::BasicBlock *conditionBlock = context.createBasicBlock("if-condition");
    llvm::BasicBlock *startTrue = context.createBasicBlock("if-true");
    llvm::BasicBlock *end = context.createBasicBlock("if-end");

    context.builder.SetInsertPoint(start);
    context.builder.CreateBr(conditionBlock);

    llvm::BasicBlock *startFalse;
    llvm::BasicBlock *endTrueBlock = trueBranch->create(context, startTrue);
    context.builder.SetInsertPoint(endTrueBlock);
    context.builder.CreateBr(end);

    if (falseBranch == nullptr) {
        startFalse = end;
    } else {
        startFalse = context.createBasicBlock("if-false");
        llvm::BasicBlock *endFalse = falseBranch->create(context, startFalse);
        context.builder.SetInsertPoint(endFalse);
        context.builder.CreateBr(end);
    }

    condition->createConditionBranch(context, conditionBlock, startTrue, startFalse);

    return end;
}