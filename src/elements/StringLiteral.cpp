//
// Created by chris on 21.01.21.
//

#include "StringLiteral.h"
#include "../parser/PrettyPrinter.h"
#include "../transform/TransformContext.h"

void StringLiteral::dump(PrettyPrinter &printer) {
    printer << *value;
}


llvm::Value *StringLiteral::createRightValue(TransformContext &context){

    //auto test = context.resetAllocBuilder().CreateAlloca(context.builder.getInt8PtrTy());
    std::string str = *value;
    str.pop_back();
    str.erase(str.begin());
    llvm::Value* globalPtr = context.builder.CreateGlobalStringPtr(str);
    //context.builder.CreateStore( globalPtr, test);
    return globalPtr;
}