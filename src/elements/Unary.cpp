//
// Created by chris on 21.01.21.
//

#include "Unary.h"
#include "../parser/PrettyPrinter.h"
#include "Operator.h"
#include "../types/TypeDefines.h"
#include "../transform/TransformContext.h"
#include "../types/Types.h"

void Unary::dump(PrettyPrinter &printer) {
    printer << "(";
    op->dump(printer);
    value->dump(printer);
    printer << ")";
}

llvm::Value *Unary::createRightValue(TransformContext &context){
    switch (op->id) {
        case PLUS:
            return value->createRightValue(context);
        case MINUS:
            return context.builder.CreateNeg(value->createRightValue(context));
        case STAR:
            return context.builder.CreateLoad(value->createRightValue(context));
        case NOT:
            return context.builder.CreateNot(value->createRightValue(context));
        case AND:
            return value->createLeftValue(context);
        case SIZEOF:
            int size = const_cast<SuperType*>(value->superType)->getSize();
            return context.builder.getInt32(size);
    }

    TRANSFORM_ERROR();
}

llvm::Value *Unary::createLeftValue(TransformContext &context){
    switch (op->id) {
        case STAR:
            return value->createRightValue(context);
    }

    TRANSFORM_ERROR();
}