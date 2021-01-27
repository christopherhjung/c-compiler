//
// Created by chris on 21.01.21.
//

#include "Assignment.h"
#include "../transform/TransformContext.h"
#include "../transform/TransformException.h"
#include "Expression.h"
#include "Number.h"

llvm::Value *Assignment::makeAssignment(TransformContext &context, Expression* left, Expression* right)  {
    llvm::Value* value = nullptr;
    if(left->semanticType->asPointerType() ){
        if(auto number = dynamic_cast<Number*>(right)){
            if(number->getValue() == 0){
                value = llvm::Constant::getNullValue(context.getType(left->semanticType));
            }
        }
    }

    if(value == nullptr){
        value = right->createRightValue(context);
    }

    if(left->semanticType->asPointerType() && right->semanticType->asPointerType() && !right->semanticType->equals(left->semanticType)){
        value = context.builder.CreatePointerCast(right->createRightValue(context), context.getType(left->semanticType));
    }

    context.builder.CreateStore(value, left->createLeftValue(context));
    return value;
}

llvm::Value *Assignment::createRightValue(TransformContext &context) {
    return makeAssignment(context, left, right);
}

