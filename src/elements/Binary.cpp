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

    if(op->id == ARROW || op->id == DOT){

        llvm::Value *value = left->createRightValue(context);
        auto aType = left->superType;

        if(op->id == ARROW){
            value = context.builder.CreateLoad(value);
            aType = aType->asPointerType()->subType;
        }

        auto structType = aType->asSuperStructType();
        auto identifier = dynamic_cast<const Identifier*>(right);

        int index = structType->map.find(identifier->value)->second;

        int offset = 0;
        for(int i = 0 ; i < index ; i++){
            auto inner = structType->types[i];
            offset += const_cast<SuperType*>(inner)->getSize();
        }

        std::vector<llvm::Value*> indices;
        indices.push_back(context.builder.getInt32(0));
        indices.push_back(context.builder.getInt32(offset));

        llvm::Value *elementPointer
                = context.builder.CreateInBoundsGEP(value, indices);

        llvm::Value *result = context.builder.CreateLoad(elementPointer);

        return result;
    }


}