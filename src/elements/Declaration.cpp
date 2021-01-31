//
// Created by chris on 21.01.21.
//

#include "Declaration.h"
#include "../parser/PrettyPrinter.h"
#include "../transform/TransformContext.h"
#include "Type.h"
#include "Declarator.h"
#include "../types/SemanticType.h"
#include "StructType.h"
#include "Assignment.h"

void Declaration::dump(PrettyPrinter &printer) {
    type->dump(printer);
    if (declarator != nullptr) {
        printer << " ";
        declarator->dump(printer);

        if (initializer != nullptr) {
            printer << " = ";
            initializer->dump(printer);
        }
    }
}

void Declaration::create(TransformContext &context) {
    if (context.functionScope) {
        llvm::Type *type = context.getType(semanticType);
        if(semanticType->identifier != nullptr){
            auto value = context.resetAllocBuilder().CreateAlloca(type);
            context.currentScope->types[semanticType->identifier->value].value = value;
            if (initializer) {
                //llvm::Value *initValue = initializer->createRightValue(context);
                //context.builder.CreateStore(initValue, value);
                Assignment::makeAssignment(context, const_cast<Identifier*>(semanticType->identifier), initializer);
            }
        }
    } else if (auto methodType = semanticType->asMethodType()) {
        auto returnType = context.getType(methodType->subType);
        std::vector<llvm::Type *> paramTypes;
        for (auto type : methodType->types) {
            if (!type->equals(VoidType)) {
                paramTypes.push_back(context.getType(type));
            }
        }
        const std::string *methodName = methodType->identifier->value;

        context.createFunctionDecl(*methodName, returnType, paramTypes);

        context.mainScope->types[methodName].value = context.currentFunction;
    } else {
        llvm::Type *type = context.getType(semanticType);
        if(semanticType->identifier){
            llvm::Constant *initValue;
            if (initializer) {
                if(type->isPointerTy()){
                    initValue = llvm::Constant::getNullValue(type);
                }else{
                    initValue = reinterpret_cast<llvm::Constant*>(initializer->createRightValue(context));
                }
            } else {
                initValue = llvm::Constant::getNullValue(type);
            }

            const std::string *name = semanticType->identifier->value;
            llvm::GlobalVariable *globalVar = new llvm::GlobalVariable(
                    context.module,
                    type,
                    false,
                    llvm::GlobalValue::ExternalLinkage,
                    initValue,
                    *name,
                    0,
                    llvm::GlobalVariable::NotThreadLocal,
                    0,
                    false);

            context.mainScope->get(name)->value = globalVar;
        }
    }
}
