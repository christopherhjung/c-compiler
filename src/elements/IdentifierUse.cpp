//
// Created by chris on 21.01.21.
//

#include "IdentifierUse.h"
#include "../parser/PrettyPrinter.h"
#include "../transform/TransformContext.h"
#include "../types/Types.h"

void IdentifierUse::dump(PrettyPrinter &printer) {
    printer << *value;
}

llvm::Value *IdentifierUse::createRightValue(TransformContext &context) {
    if(semanticType->asMethodType()){
        return identifier->llvmValue;
    }else{
        return context.builder.CreateLoad(identifier->llvmValue);
    }
}

llvm::Value *IdentifierUse::createLeftValue(TransformContext &context) {
    return identifier->llvmValue;
}

const SemanticType *IdentifierUse::getType() {
    return identifier->getType();
}
