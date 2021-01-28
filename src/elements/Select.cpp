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
    auto indexValue = index->createRightValue(context);
    auto indexValue64 = context.builder.CreateSExtOrTrunc(indexValue, context.builder.getInt64Ty());
    auto basePointer = target->createRightValue(context);
    auto elemPointer = context.builder.CreateGEP(basePointer, indexValue64);
    return elemPointer;
}


llvm::Value *Select::createRightValue(TransformContext &context)
{
    return context.builder.CreateLoad(createLeftValue(context));
}