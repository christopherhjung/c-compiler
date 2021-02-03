//
// Created by chris on 21.01.21.
//

#include "Identifier.h"
#include "../parser/PrettyPrinter.h"
#include "../transform/TransformContext.h"
#include "../types/Types.h"

void Identifier::dump(PrettyPrinter &printer) {
    printer << *value;
}

llvm::Value *Identifier::createRightValue(TransformContext &context) {
    if(semanticType->asMethodType()){
        return anchor->value;
    }else{
        return context.builder.CreateLoad(anchor->value);
    }
}

llvm::Value *Identifier::createLeftValue(TransformContext &context) {
    return anchor->value;
}