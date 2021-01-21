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
    llvm::Value *randomVariable = context.resetAllocBuilder().CreateAlloca(context.builder.getInt32Ty());
    return context.builder.CreateLoad(randomVariable);
}

llvm::Value *Identifier::createLeftValue(TransformContext &context) {
    llvm::Value *randomVariable = context.resetAllocBuilder().CreateAlloca(context.builder.getInt32Ty());
    return randomVariable;
}