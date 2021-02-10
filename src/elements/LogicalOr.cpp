//
// Created by chris on 21.01.21.
//

#include "LogicalOr.h"
#include "../parser/PrettyPrinter.h"
#include "../transform/TransformContext.h"

void
LogicalOr::createConditionBranch(TransformContext &context, llvm::BasicBlock *trueBlock,
                                 llvm::BasicBlock *falseBlock) {
    auto extraBB = context.createBasicBlock("logic-or-temp");
    left->createConditionBranch(context, trueBlock, extraBB);
    context.setCurrentBlock(extraBB);
    right->createConditionBranch(context, trueBlock, falseBlock);
}

