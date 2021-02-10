//
// Created by chris on 21.01.21.
//

#include "Constant.h"
#include "../parser/PrettyPrinter.h"
#include "../transform/TransformContext.h"
#include "Util.h"

void Constant::dump(PrettyPrinter &printer) {
    printer << *value;
}

llvm::Value *Constant::createRightValue(TransformContext &context){
    std::string str = escapeString(*value);
    return context.builder.getInt8(str[0]);
}

llvm::Value *Constant::getSize(TransformContext &context){
    return context.builder.getInt32(4);
}