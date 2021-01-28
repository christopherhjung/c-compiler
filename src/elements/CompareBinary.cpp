//
// Created by chris on 21.01.21.
//

#include "CompareBinary.h"
#include "../types/TypeDefines.h"
#include "../parser/PrettyPrinter.h"
#include "../transform/TransformContext.h"
#include "Operator.h"

void CompareBinary::createConditionBranch(TransformContext &context,
                                          llvm::BasicBlock *trueBlock, llvm::BasicBlock *falseBlock) {
    /*llvm::Value *condition = context.builder.CreateICmpNE(createRightValue(context), context.builder.getInt32(0),
                                                          "condition");*/

    context.builder.CreateCondBr(makeCompare(context), trueBlock, falseBlock);
}

llvm::CmpInst::Predicate CompareBinary::getPredicate() {
    llvm::CmpInst::Predicate predicate;
    switch (this->op->id) {
        case EQUAL:
            predicate = llvm::CmpInst::Predicate::ICMP_EQ;
            break;
        case NOT_EQUAL:
            predicate = llvm::CmpInst::Predicate::ICMP_NE;
            break;
        case LESS:
            predicate = llvm::CmpInst::Predicate::ICMP_SLT;
            break;
        case LESS_EQUAL:
            predicate = llvm::CmpInst::Predicate::ICMP_SLE;
            break;
        case GREATER:
            predicate = llvm::CmpInst::Predicate::ICMP_SGT;
            break;
        case GREATER_EQUAL:
            predicate = llvm::CmpInst::Predicate::ICMP_SGE;
            break;
        default:
            TRANSFORM_ERROR();
    }

    return predicate;
}

llvm::Value *CompareBinary::makeCompare(TransformContext &context) {

    llvm::Value* leftValue = left->createRightValue(context);
    if(left->semanticType->asPointerType()){
        leftValue = context.builder.CreatePtrToInt(leftValue, context.builder.getInt32Ty());
    }

    llvm::Value* rightValue = right->createRightValue(context);
    if(right->semanticType->asPointerType()){
        rightValue = context.builder.CreatePtrToInt(leftValue, context.builder.getInt32Ty());
    }


    llvm::Value* resultValue = context.builder.CreateICmp(getPredicate(), leftValue , rightValue);


    if(resultValue->getType()->isIntegerTy(1)){
        resultValue = resultValue;
    }

    return resultValue;
}

llvm::Value *CompareBinary::createRightValue(TransformContext &context) {

    llvm::Value* resultValue = makeCompare(context);
    resultValue = context.builder.CreateIntCast(resultValue, context.builder.getInt32Ty(), false);
    //resultValue = context.builder.CreateTrunc(resultValue, context.builder.getInt32Ty());
    return resultValue;
}