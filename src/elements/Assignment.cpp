//
// Created by chris on 21.01.21.
//

#include "Assignment.h"
#include "../transform/TransformContext.h"
#include "../transform/TransformException.h"

llvm::Value* Assignment::createRightValue(TransformContext &context) {
return context.builder.CreateStore(right->createRightValue(context), left->createLeftValue(context));
}

llvm::Value* Assignment::create(TransformContext &context)  {
    TRANSFORM_ERROR();
}

llvm::BasicBlock* Assignment::create(TransformContext &context, llvm::BasicBlock* start) {
    context.builder.SetInsertPoint(start);
    createRightValue(context);
    return start;
}