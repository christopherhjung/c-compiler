//
// Created by chris on 21.01.21.
//

#include "Declaration.h"
#include "../parser/PrettyPrinter.h"
#include "../transform/TransformContext.h"
#include "Type.h"
#include "Declarator.h"

void Declaration::dump(PrettyPrinter &printer) {
    type->dump(printer);
    if (declarator != nullptr) {
        printer << " ";
        declarator->dump(printer);
    }
}

llvm::Value *Declaration::create(TransformContext &context) {
    return context.resetAllocBuilder().CreateAlloca(context.builder.getInt32Ty());
}

llvm::BasicBlock *Declaration::create(TransformContext &context, llvm::BasicBlock *start) {
    context.builder.SetInsertPoint(start);
    create(context);
    return start;
}