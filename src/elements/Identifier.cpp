//
// Created by chris on 21.01.21.
//

#include "Identifier.h"
#include "../parser/PrettyPrinter.h"
#include "../transform/TransformContext.h"

void Identifier::dump(PrettyPrinter &printer) {
    printer << *value;
}

llvm::Value *Identifier::createRightValue(TransformContext &context) {
    llvm::Value *variable = context.currentScope->get(value)->value;
    return context.builder.CreateLoad(variable);
}

llvm::Value *Identifier::createLeftValue(TransformContext &context) {
    return context.currentScope->get(value)->value;
}