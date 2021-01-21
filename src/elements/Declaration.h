//
// Created by chris on 21.01.21.
//

#pragma once

#include "Elements.h"

class Declaration : public Statement{
public:
    Type* type = nullptr;
    Declarator* declarator = nullptr;

    void dump(PrettyPrinter& printer) override{
        type->dump(printer);
        if(declarator != nullptr){
            printer << " ";
            declarator->dump(printer);
        }
    }

    llvm::Value* create(TransformContext &context) override{
        return context.resetAllocBuilder().CreateAlloca(context.builder.getInt32Ty());
    }

    llvm::BasicBlock* create(TransformContext &context, llvm::BasicBlock* start) override{
        context.builder.SetInsertPoint(start);
        create(context);
        return start;
    }
};



