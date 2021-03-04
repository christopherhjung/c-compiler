//
// Created by chris on 21.01.21.
//

#include "Select.h"
#include "../parser/PrettyPrinter.h"
#include "../transform/TransformContext.h"

void Select::dump(PrettyPrinter &printer) {
    printer << "(";
    target->dump(printer);
    printer << "[";
    index->dump(printer);
    printer << "]";
    printer << ")";
}



llvm::Value *Select::createLeftValue(TransformContext &context){
    Expression *left;
    Expression *right;

    if(target->getType()->asPointerType()){
        left = target;
        right = index;
    }else{
        left = index;
        right = target;
    }

    auto indexValue = right->createRightValue(context);
    auto indexValue64 = context.builder.CreateSExtOrTrunc(indexValue, context.builder.getInt64Ty());
    auto basePointer = left->createRightValue(context);
    auto elemPointer = context.builder.CreateGEP(basePointer, indexValue64);
    return elemPointer;
}


llvm::Value *Select::createRightValue(TransformContext &context)
{
    return context.builder.CreateLoad(createLeftValue(context));
}