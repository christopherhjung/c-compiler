//
// Created by chris on 21.01.21.
//

#include "LogicalAnd.h"
#include "../parser/PrettyPrinter.h"
#include "../transform/TransformContext.h"

void
LogicalAnd::createConditionBranch(TransformContext &context, llvm::BasicBlock *trueBlock,
                                  llvm::BasicBlock *falseBlock) {
    auto extraBB = context.createBasicBlock("logic-and-temp");
    left->createConditionBranch(context, extraBB, falseBlock);
    context.setCurrentBlock(extraBB);
    right->createConditionBranch(context, trueBlock, falseBlock);
}

