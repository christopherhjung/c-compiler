//
// Created by chris on 21.01.21.
//

#include "Call.h"

#include "../parser/PrettyPrinter.h"
#include "../transform/TransformContext.h"

void Call::dump(PrettyPrinter &printer) {
    printer << "(";
    target->dump(printer);
    printer << "(";
    bool first = true;
    for (auto val : values) {
        if (first) {
            first = false;
        } else {
            printer << ", ";
        }
        val->dump(printer);
    }
    printer << ")";
    printer << ")";
}

llvm::Value *Call::createRightValue(TransformContext &context){

    std::vector<llvm::Value*> arguments;
    for( auto value : values ){
        arguments.push_back(value->createRightValue(context));
    }

    llvm::Value* value = target->createLeftValue(context);

    llvm::Type* type = value->getType();
    auto functionSuperType = target->superType;
    if(type->getNumContainedTypes() == 1 && type->getContainedType(0)->isPointerTy()){
        value = context.builder.CreateLoad(value);
        functionSuperType = functionSuperType->asPointerType()->subType;
    }

    auto functionType = reinterpret_cast<llvm::FunctionType*>(context.getType(functionSuperType));
    auto function = llvm::FunctionCallee(functionType,value);
    return context.builder.CreateCall(function, arguments);
}