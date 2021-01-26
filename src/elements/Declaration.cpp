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
    if(context.functionScope){
        auto value = context.resetAllocBuilder().CreateAlloca(context.getType(superType));
        context.currentScope->types[superType->identifier->value].value = value;
    }else if(auto methodType = superType->asMethodType()){


        auto returnType = context.getType(methodType->subType);
        std::vector<llvm::Type*> paramTypes;
        for(auto type : methodType->types){
            if(!type->equals(VoidType)){
                paramTypes.push_back(context.getType(type));
            }
        }
        const std::string *methodName = methodType->identifier->value;

        context.createFunctionDecl(*methodName, returnType, paramTypes);

        context.mainScope->functions[methodName] = context.currentFunction;
    }
}
