//
// Created by chris on 21.01.21.
//

#include "Assignment.h"
#include "../transform/TransformContext.h"
#include "../transform/TransformException.h"
#include "Expression.h"
#include "Number.h"

llvm::Value *Assignment::ensureAssignment(TransformContext &context, const SemanticType* leftType, Expression* right)  {
    llvm::Value* value = nullptr;
    if(leftType->asPointerType() ){
        if(auto number = dynamic_cast<Number*>(right)){
            if(number->getValue() == 0){
                value = llvm::Constant::getNullValue(context.getType(leftType));
            }
        }
    }

    if(value == nullptr){
        value = right->createRightValue(context);
    }

    if(!right->getType()->equals(leftType)){
        if(leftType->asPointerType() && right->getType()->asPointerType() ){
            value = context.builder.CreatePointerCast(value, context.getType(leftType));
        }else if(leftType->asSimpleType() && right->getType()->asSimpleType()){
            value = context.builder.CreateIntCast(value, context.getType(leftType), true);
        }
    }

    return value;
}

llvm::Value *Assignment::makeAssignment(TransformContext &context, Identifier* left, Expression* right)  {
    llvm::Value* value = ensureAssignment(context, left->getType(), right);
    context.builder.CreateStore(value, left->llvmValue);
    return value;
}

llvm::Value *Assignment::makeAssignment(TransformContext &context, Expression* left, Expression* right)  {
    llvm::Value* value = ensureAssignment(context, left->getType(), right);
    context.builder.CreateStore(value, left->createLeftValue(context));
    return value;
}

llvm::Value *Assignment::createRightValue(TransformContext &context) {
    return makeAssignment(context, left, right);
}

