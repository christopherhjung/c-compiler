//
// Created by chris on 21.01.21.
//

#include "Expression.h"
#include "../parser/PrettyPrinter.h"
#include "../transform/TransformException.h"
#include "../transform/TransformContext.h"

llvm::Value *Expression::createRightValue(TransformContext &context) {
    TRANSFORM_ERROR();
};

llvm::Value *Expression::createLeftValue(TransformContext &context) {
    TRANSFORM_ERROR();
};

void
Expression::createConditionBranch(TransformContext &context, llvm::BasicBlock *trueBlock,
                                  llvm::BasicBlock *falseBlock) {
    llvm::Value *condition = context.builder.CreateICmpNE(createRightValue(context), context.builder.getInt32(0));
    context.builder.CreateCondBr(condition, trueBlock, falseBlock);
}