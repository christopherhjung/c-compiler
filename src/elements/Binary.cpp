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
#include "Assignment.h"

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
    if(op->id == ARROW || op->id == DOT){
        return context.builder.CreateLoad(createLeftValue(context));
    }

    llvm::Value* leftValue = left->createRightValue(context);
    llvm::Value* rightValue = right->createRightValue(context);

    switch (op->id) {
        case PLUS:
            if(opInfo == 0){
                leftValue = context.builder.CreateIntCast(leftValue, context.builder.getInt32Ty(), true);
                rightValue = context.builder.CreateIntCast(rightValue, context.builder.getInt32Ty(), true);
                return context.builder.CreateAdd(leftValue, rightValue);
            }else if (opInfo == 1){
                return context.builder.CreateGEP(leftValue, rightValue);
            }else if (opInfo == 2){
                return context.builder.CreateGEP(rightValue, leftValue);
            }
        case MINUS:
            if(opInfo == 0){
                return context.builder.CreateSub(leftValue, rightValue);
            }else if (opInfo == 1){
                return context.builder.CreateGEP(leftValue, context.builder.CreateNeg(rightValue));
            }else if (opInfo == 2){
                llvm::Value* diff = context.builder.CreatePtrDiff(leftValue, rightValue);
                return context.builder.CreateTrunc(diff, context.builder.getInt32Ty());
            }
        case STAR:
            return context.builder.CreateMul(leftValue, rightValue);
        case LEFT_SHIFT:
            return context.builder.CreateShl(leftValue, rightValue);
        case RIGHT_SHIFT:
            return context.builder.CreateAShr(leftValue, rightValue);
    }

    TRANSFORM_ERROR();
}

llvm::Value *Binary::createLeftValue(TransformContext &context){
    if(op->id == ARROW || op->id == DOT){
        llvm::Value *value = left->createLeftValue(context);
        auto aType = left->semanticType;

        if(op->id == ARROW){
            value = context.builder.CreateLoad(value);
            aType = aType->asPointerType()->subType;
        }

        auto structType = aType->asSuperStructType();
        auto identifier = dynamic_cast<const Identifier*>(right);

        int index = structType->map.find(identifier->value)->second;

        std::vector<llvm::Value*> indices;
        indices.push_back(context.builder.getInt32(0));
        indices.push_back(context.builder.getInt32(index));

        llvm::Value *elementPointer
                = context.builder.CreateInBoundsGEP(value, indices);

        return elementPointer;
    }

    TRANSFORM_ERROR();
}