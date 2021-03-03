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

    auto commonType = context.getGreatestType(left->getType(), right->getType());

    llvm::Value* leftValue = left->createRightValue(context);
    if(left->getType()->asPointerType()){
        leftValue = context.builder.CreatePtrToInt(leftValue, commonType);
    }else{
        leftValue = context.builder.CreateIntCast(leftValue, commonType, true);
    }

    llvm::Value* rightValue = right->createRightValue(context);
    if(right->getType()->asPointerType()){
        rightValue = context.builder.CreatePtrToInt(rightValue, commonType);
    }else{
        rightValue = context.builder.CreateIntCast(rightValue, commonType, true);
    }

    llvm::Value* resultValue = context.builder.CreateICmp(getPredicate(), leftValue , rightValue);

    return resultValue;
}

llvm::Value *CompareBinary::createRightValue(TransformContext &context) {

    llvm::Value* resultValue = makeCompare(context);
    resultValue = context.builder.CreateIntCast(resultValue, context.builder.getInt32Ty(), false);
    //resultValue = context.builder.CreateTrunc(resultValue, context.builder.getInt32Ty());
    return resultValue;
}