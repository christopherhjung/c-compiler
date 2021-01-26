//
// Created by chris on 21.01.21.
//

#include "Sizeof.h"
#include "../parser/PrettyPrinter.h"
#include "Type.h"
#include "Declarator.h"
#include "../types/Types.h"
#include "../transform/TransformContext.h"

void Sizeof::dump(PrettyPrinter &printer) {
    printer << "(";
    printer << "sizeof(";
    type->dump(printer);
    if (declarator != nullptr) {
        declarator->dump(printer);
    }
    printer << ")";
    printer << ")";
}

llvm::Value *Sizeof::createRightValue(TransformContext &context){
    llvm::Type* type = context.getType(inner);
    llvm::DataLayout layout(&context.module);
    unsigned int size = layout.getTypeAllocSize(type);
    return context.builder.getInt32(size );
}