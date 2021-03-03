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
#include "Sizeof.h"

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

    if(opInfo == 0){
        leftValue = context.builder.CreateIntCast(leftValue, context.builder.getInt32Ty(), true);
        rightValue = context.builder.CreateIntCast(rightValue, context.builder.getInt32Ty(), true);
    }

    switch (op->id) {
        case PLUS:
            if(opInfo == 0){
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
                /*auto leftInt = context.builder.CreatePtrToInt(leftValue, context.builder.getInt64Ty());
                auto rightInt = context.builder.CreatePtrToInt(rightValue, context.builder.getInt64Ty());

                auto bytesDiff = context.builder.CreateSub(leftInt, rightInt);

                const SemanticType *targetType = left->semanticType->asMethodType();

                if(!targetType){
                    targetType = left->semanticType->asPointerType()->subType;
                }

                auto size = Sizeof::getInt64Size(context, targetType);
                auto diff = context.builder.CreateSDiv(bytesDiff, size );

                return context.builder.CreateTrunc(diff, context.builder.getInt32Ty());*/

                return context.builder.CreateTrunc(context.builder.CreatePtrDiff(leftValue, rightValue), context.builder.getInt32Ty());
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
        auto aType = left->getType();

        if(op->id == ARROW){
            value = context.builder.CreateLoad(value);
            aType = aType->asPointerType()->subType;
        }

        auto structType = aType->asSemanticStructType();
        auto identifier = dynamic_cast<const IdentifierUse*>(right);

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