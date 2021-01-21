//
// Created by chris on 21.01.21.
//

#include "Number.h"
#include "../parser/PrettyPrinter.h"
#include "../transform/TransformContext.h"

void Number::dump(PrettyPrinter &printer) {
    printer << *value;
}

llvm::Value *Number::createRightValue(TransformContext &context) {
    return context.builder.getInt32(1);
}