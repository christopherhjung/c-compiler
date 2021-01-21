//
// Created by chris on 21.01.21.
//

#include "Binary.h"
#include "../types/TypeDefines.h"
#include "../parser/PrettyPrinter.h"
#include "Expression.h"
#include "Operator.h"
#include "Operator.h"
#include "../types/TypeDefines.h"
#include "../transform/TransformContext.h"

void Binary::dump(PrettyPrinter &printer) {
    printer << "(";
    left->dump(printer);
    if (op->id != ARROW && op->id != DOT) {
        printer << " ";
    }
    op->dump(printer);
    if (op->id != ARROW && op->id != DOT) {
        printer << " ";
    }
    right->dump(printer);
    printer << ")";
}

llvm::Value *Binary::createRightValue(TransformContext &context) {
    switch (op->id) {
        case PLUS:
            return context.builder.CreateAdd(left->createRightValue(context), right->createRightValue(context));
        case MINUS:
            return context.builder.CreateSub(left->createRightValue(context), right->createRightValue(context));
        case STAR:
            return context.builder.CreateMul(left->createRightValue(context), right->createRightValue(context));
        case LEFT_SHIFT:
            return context.builder.CreateShl(left->createRightValue(context), right->createRightValue(context));
        case RIGHT_SHIFT:
            return context.builder.CreateAShr(left->createRightValue(context), right->createRightValue(context));
    }
}