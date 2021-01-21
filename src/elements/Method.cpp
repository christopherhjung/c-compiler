//
// Created by chris on 21.01.21.
//

#include "Method.h"
#include "../parser/PrettyPrinter.h"
#include "../transform/TransformContext.h"
#include "Block.h"
#include "Declaration.h"
#include "../types/SuperType.h"

void Method::dump(PrettyPrinter &printer) {
    declaration->dump(printer);
    printer.newLine();
    body->dump(printer);
}

void Method::create(TransformContext &context) {
    auto methodType = declaration->superType->asMethodType();

    auto returnType = context.getType(methodType->subType);
    std::vector<llvm::Type*> paramTypes;
    for(auto type : methodType->types){
        if(!type->equals(VoidType)){
            paramTypes.push_back(context.getType(type));
        }
    }
    const std::string *methodName = declaration->superType->identifier->value;

    auto entry = context.createFunction(*methodName, returnType, paramTypes);

    auto args = context.currentFunction->arg_begin();
    for(auto type : methodType->types){
        llvm::Argument *arg = args;
        const std::string* name = type->identifier->value;
        if(!type->equals(VoidType)){
            arg->setName(*name);
            llvm::Value *argPtr = context.resetAllocBuilder().CreateAlloca(arg->getType());
            context.builder.CreateStore(arg, argPtr);
            body->scope->get(name)->value = argPtr;
        }

        args++;
    }

    context.mainScope->functions[methodName] = context.currentFunction;

    context.setCurrentBlock(entry);
    body->create(context);
}