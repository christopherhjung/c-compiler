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

    const std::string* name = dynamic_cast<const Identifier*>(target)->value;

    auto function = context.currentScope->getFunction(name);
    return context.builder.CreateCall(function, arguments);
}