//
// Created by chris on 21.01.21.
//

#include "Declaration.h"
#include "../parser/PrettyPrinter.h"
#include "../transform/TransformContext.h"
#include "Type.h"
#include "Declarator.h"
#include "../types/SuperType.h"

void Declaration::dump(PrettyPrinter &printer) {
    type->dump(printer);
    if (declarator != nullptr) {
        printer << " ";
        declarator->dump(printer);
    }
}

void Declaration::create(TransformContext &context) {
    auto value = context.resetAllocBuilder().CreateAlloca(context.getType(superType));
    context.currentScope->types[superType->identifier->value].value = value;
}
