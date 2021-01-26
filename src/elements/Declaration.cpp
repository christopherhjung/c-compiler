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

void Declaration::dump(PrettyPrinter &printer) {
    type->dump(printer);
    if (declarator != nullptr) {
        printer << " ";
        declarator->dump(printer);
    }
}

void Declaration::create(TransformContext &context) {
    if(context.functionScope){
        llvm::Type* type = context.getType(semanticType);
        auto value = context.resetAllocBuilder().CreateAlloca(type);
        context.currentScope->types[semanticType->identifier->value].value = value;
    }else if(auto methodType = semanticType->asMethodType()){
        auto returnType = context.getType(methodType->subType);
        std::vector<llvm::Type*> paramTypes;
        for(auto type : methodType->types){
            if(!type->equals(VoidType)){
                paramTypes.push_back(context.getType(type));
            }
        }
        const std::string *methodName = methodType->identifier->value;

        context.createFunctionDecl(*methodName, returnType, paramTypes);

        context.mainScope->types[methodName].value = context.currentFunction;
    }else if(auto structType = dynamic_cast<StructType*>(type)){
        context.getType(semanticType);
    }else{
        llvm::Type* type = context.getType(semanticType);
        const std::string *name = semanticType->identifier->value;
        llvm::GlobalVariable *globalVar = new llvm::GlobalVariable(
                context.module                                       /* Module & */,
                type                              /* Type * */,
                false                                   /* bool isConstant */,
                llvm::GlobalValue::CommonLinkage              /* LinkageType */,
                llvm::Constant::getNullValue(type)      /* Constant * Initializer */,
                *name                                /* const Twine &Name = "" */,
                /* --------- We do not need this part (=> use defaults) ---------- */
                0                                       /* GlobalVariable *InsertBefore = 0 */,
                llvm::GlobalVariable::NotThreadLocal          /* ThreadLocalMode TLMode = NotThreadLocal */,
                0                                       /* unsigned AddressSpace = 0 */,
                false                                   /* bool isExternallyInitialized = false */);

        context.mainScope->get(name)->value = globalVar;
    }
}
