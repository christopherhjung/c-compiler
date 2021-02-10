//
// Created by chris on 21.01.21.
//

#include "StringLiteral.h"
#include "../parser/PrettyPrinter.h"
#include "../transform/TransformContext.h"
#include "Util.h"

void StringLiteral::dump(PrettyPrinter &printer) {
    printer << *value;
}

llvm::Value *StringLiteral::createRightValue(TransformContext &context){
    std::string str = escapeString(*value);
    llvm::Value* globalPtr = context.builder.CreateGlobalStringPtr(str);
    return globalPtr;
}

int StringLiteral::getSize(TransformContext &context){
    return value->length();
}