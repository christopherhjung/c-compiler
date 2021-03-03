//
// Created by chris on 21.01.21.
//

#include "Expression.h"
#include "../parser/PrettyPrinter.h"
#include "../transform/TransformException.h"
#include "../transform/TransformContext.h"
#include "Sizeof.h"

llvm::Value *Expression::createRightValue(TransformContext &context) {
    TRANSFORM_ERROR();
}

llvm::Value *Expression::createLeftValue(TransformContext &context) {
    TRANSFORM_ERROR();
}

void Expression::createConditionBranch(TransformContext &context, llvm::BasicBlock *trueBlock, llvm::BasicBlock *falseBlock) {

    llvm::Value* value = createRightValue(context);
    if(value->getType()->isPointerTy()){
        value = context.builder.CreatePtrToInt(value, context.builder.getInt32Ty());
    }

    value = context.builder.CreateIntCast(value, context.builder.getInt32Ty(), true);
    llvm::Value *condition = context.builder.CreateICmpNE(value, context.builder.getInt32(0));
    context.builder.CreateCondBr(condition, trueBlock, falseBlock);
}

void Expression::create(TransformContext &context) {
    createRightValue(context);
}

llvm::Value * Expression::getSize(TransformContext &context){
    return Sizeof::getInt32Size(context, getType());
}