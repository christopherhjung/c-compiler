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

void If::create(TransformContext &context) {
    llvm::BasicBlock *conditionBlock = context.currentBlock;//context.createBasicBlock("if-condition");
    llvm::BasicBlock *startTrue = context.createBasicBlock("if-true");
    llvm::BasicBlock *end = context.createBasicBlock("if-end");

    //context.builder.CreateBr(conditionBlock);

    llvm::BasicBlock *startFalse;
    context.setCurrentBlock(startTrue);
    trueBranch->create(context);
    context.builder.CreateBr(end);

    if (falseBranch == nullptr) {
        startFalse = end;
    } else {
        startFalse = context.createBasicBlock("if-false");
        context.setCurrentBlock(startFalse);
        falseBranch->create(context);
        context.builder.CreateBr(end);
    }

    context.setCurrentBlock(conditionBlock);
    condition->createConditionBranch(context, startTrue, startFalse);

    context.setCurrentBlock(end);
}