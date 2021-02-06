//
// Created by chris on 21.01.21.
//

#include "Unary.h"
#include "../parser/PrettyPrinter.h"
#include "Operator.h"
#include "../types/TypeDefines.h"
#include "../transform/TransformContext.h"
#include "../types/Types.h"
#include "Sizeof.h"

void Unary::dump(PrettyPrinter &printer) {
    printer << "(";
    op->dump(printer);
    value->dump(printer);
    printer << ")";
}

llvm::Value *Unary::createRightValue(TransformContext &context){
    switch (op->id) {
        case PLUS:
            return value->createRightValue(context);
        case MINUS:
            return context.builder.CreateNeg(value->createRightValue(context));
        case STAR:{
            llvm::Value *variable = value->createRightValue(context);

            if(value->semanticType->asMethodType() || value->semanticType->equals(MethodPointerType)){
                return variable;
            }else{
                return context.builder.CreateLoad(variable);
            }
        }
        case NOT:
        {
            llvm::Value* rightValue = value->createRightValue(context);
            if(rightValue->getType()->isPointerTy()){
                rightValue = context.builder.CreatePtrToInt(rightValue, context.builder.getInt32Ty());
            }

            return context.builder.CreateIntCast(
                    context.builder.CreateICmpEQ(rightValue, context.builder.getInt32(0)), context.builder.getInt32Ty(), false);
        }
        case AND:
            return value->createLeftValue(context);
        case SIZEOF:
            return Sizeof::getInt32Size(context, value->semanticType);
    }

    TRANSFORM_ERROR();
}

llvm::Value *Unary::createLeftValue(TransformContext &context){
    switch (op->id) {
        case STAR:
            return value->createRightValue(context);
    }

    TRANSFORM_ERROR();
}

void Unary::createConditionBranch(TransformContext &context, llvm::BasicBlock *trueBlock,
                      llvm::BasicBlock *falseBlock){
    switch (op->id) {case NOT:
        {
            llvm::Value* rightValue = value->createRightValue(context);
            if(rightValue->getType()->isPointerTy()){
                rightValue = context.builder.CreatePtrToInt(rightValue, context.builder.getInt32Ty());
            }

            llvm::Value* cmp = context.builder.CreateICmpEQ(rightValue, context.builder.getInt32(0));

            context.builder.CreateCondBr(cmp, trueBlock, falseBlock);
        }
        return;
    }

    return Expression::createConditionBranch(context, trueBlock, falseBlock);
}