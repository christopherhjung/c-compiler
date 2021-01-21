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
    llvm::Value *condition = context.builder.CreateICmpNE(createRightValue(context), context.builder.getInt32(0),
                                                          "condition");
    context.builder.CreateCondBr(condition, trueBlock, falseBlock);
}

llvm::CmpInst::Predicate CompareBinary::getPredicate() {
    llvm::CmpInst::Predicate predicate = llvm::CmpInst::Predicate::BAD_ICMP_PREDICATE;
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
    }

    return predicate;
}

llvm::Value *CompareBinary::createRightValue(TransformContext &context) {
    return context.builder.CreateICmp(getPredicate(), left->createRightValue(context), right->createRightValue(context));
}