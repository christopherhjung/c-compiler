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

    auto functionSemanticType = target->semanticType;

    llvm::Value* value;
    if(functionSemanticType->asMethodType()){
        value = target->createLeftValue(context);
    }else{
        value = target->createRightValue(context);
    }

    const MethodType* methodType;
    if(auto pointerType = functionSemanticType->asPointerType()){
        if(!pointerType->subType->asMethodType()){
            functionSemanticType = pointerType->subType;
        }

        methodType = functionSemanticType->asPointerType()->subType->asMethodType();
    }else{
        methodType = functionSemanticType->asMethodType();
    }


    std::vector<llvm::Value*> arguments;
    arguments.reserve(values.size());
    for( int i = 0 ; i < values.size() ; i++){
        arguments.push_back(Assignment::ensureAssignment(context, methodType->types[i], values[i]));
    }

    auto functionType = reinterpret_cast<llvm::FunctionType*>(context.getType(methodType));
    auto function = llvm::FunctionCallee(functionType,value);
    return context.builder.CreateCall(function, arguments);
}