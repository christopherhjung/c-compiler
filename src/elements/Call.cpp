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

    //auto methodType = target->superType->asMethodType();
    //std::string* name = methodType->identifier->value;

    llvm::Value* value = target->createLeftValue(context);


    const std::string* name = dynamic_cast<const Identifier*>(target)->value;

    auto function = context.currentScope->get(name)->function;

    return context.builder.CreateCall(function, arguments);




/*
    auto methodType = target->superType->asMethodType();
    std::string* name = methodType->identifier->value;



    llvm::Value* functionValue = target->createLeftValue(context);



    if(auto function = dynamic_cast<llvm::FunctionCallee*>(functionValue)){

    }

    return context.builder.CreateCall(, arguments);*/
}