//
// Created by chris on 21.01.21.
//

#include "Call.h"

#include "../parser/PrettyPrinter.h"
#include "../transform/TransformContext.h"
#include "Assignment.h"

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

    auto functionSemanticType = target->getType();

    llvm::Value* value = target->createRightValue(context);

    const MethodType* methodType = functionSemanticType->unpackMethodType();

    std::vector<llvm::Value*> arguments;
    arguments.reserve(values.size());
    for( size_t i = 0 ; i < values.size() ; i++){
        arguments.push_back(Assignment::ensureAssignment(context, methodType->types[i], values[i]));
    }

    auto functionType = llvm::dyn_cast<llvm::FunctionType>(context.getType(methodType));
    auto function = llvm::FunctionCallee(functionType,value);


    return context.builder.CreateCall(function, arguments);
}