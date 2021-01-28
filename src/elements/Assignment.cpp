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
        if(right->deref){
            value = right->createLeftValue(context);
        }else{
            value = right->createRightValue(context);
        }
    }

    if(!right->semanticType->equals(leftType)){
        if(leftType->asPointerType() && right->semanticType->asPointerType() ){
            value = context.builder.CreatePointerCast(right->createRightValue(context), context.getType(leftType));
        }else if(leftType->asSimpleType() && right->semanticType->asSimpleType()){
            value = context.builder.CreateIntCast(right->createRightValue(context), context.getType(leftType), true);
        }
    }

    return value;
}

llvm::Value *Assignment::makeAssignment(TransformContext &context, Expression* left, Expression* right)  {
    llvm::Value* value = ensureAssignment(context, left->semanticType, right);
    context.builder.CreateStore(value, left->createLeftValue(context));
    return value;
}

llvm::Value *Assignment::createRightValue(TransformContext &context) {
    return makeAssignment(context, left, right);
}

