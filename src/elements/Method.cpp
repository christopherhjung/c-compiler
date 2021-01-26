//
// Created by chris on 21.01.21.
//

#include "Method.h"
#include "../parser/PrettyPrinter.h"
#include "../transform/TransformContext.h"
#include "Block.h"
#include "Declaration.h"
#include "../types/SuperType.h"
#include "Return.h"

void Method::dump(PrettyPrinter &printer) {
    declaration->dump(printer);
    printer.newLine();
    body->dump(printer);
}

void Method::create(TransformContext &context) {
    auto methodType = declaration->superType->asMethodType();

    std::vector<llvm::Type*> paramTypes;
    for(auto type : methodType->types){
        if(!type->equals(VoidType)){
            paramTypes.push_back(context.getType(type));
        }
    }
    const std::string *methodName = declaration->superType->identifier->value;

    llvm::Function* declared = reinterpret_cast<llvm::Function*>(context.mainScope->types[methodName].value);

    if(!declared){
        declaration->create(context);
        declared = reinterpret_cast<llvm::Function*>(context.mainScope->types[methodName].value);
    }

    llvm::BasicBlock* entry;

    entry = llvm::BasicBlock::Create(context.llvmContext, "entry", declared, 0);
    context.currentFunction = declared;
    context.functionScope = body->scope;
    context.setCurrentBlock(entry);
    context.allocBuilder.SetInsertPoint(entry);

    auto args = context.currentFunction->arg_begin();
    for(auto type : methodType->types){
        llvm::Argument *arg = args;
        if(!type->equals(VoidType)){
            const std::string* name = type->identifier->value;
            arg->setName(*name);
            llvm::Value *argPtr = context.resetAllocBuilder().CreateAlloca(arg->getType());
            context.builder.CreateStore(arg, argPtr);
            body->scope->get(name)->value = argPtr;
        }

        args++;
    }

    body->create(context);

    if(body->children.empty() || !dynamic_cast<Return*>(body->children[body->children.size() - 1])){
        llvm::Type *returnType = context.builder.getCurrentFunctionReturnType();
        if (returnType->isVoidTy()) {
            context.builder.CreateRetVoid();
        } else {
            context.builder.CreateRet(llvm::Constant::getNullValue(declared->getReturnType()));
        }
    }

    context.functionScope = nullptr;
}