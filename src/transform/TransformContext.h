#pragma once

#include "llvm/IR/Module.h"                /* Module */
#include "llvm/IR/Function.h"              /* Function */
#include "llvm/IR/IRBuilder.h"             /* IRBuilder */
#include "llvm/IR/LLVMContext.h"           /* LLVMContext */
#include "llvm/IR/GlobalValue.h"           /* GlobaleVariable, LinkageTypes */
#include "llvm/IR/Verifier.h"              /* verifyFunction, verifyModule */
#include "llvm/Support/Signals.h"          /* Nice stacktrace output */
#include "llvm/Support/SystemUtils.h"
#include "llvm/Support/PrettyStackTrace.h"

#include <queue>
#include "../semantic/Scope.h"

class TransformContext {
    llvm::IRBuilder<> &allocBuilder;
public:
    llvm::LLVMContext &llvmContext;
    llvm::Module &module;
    llvm::IRBuilder<> &builder;

    llvm::Function *currentFunction;
    llvm::BasicBlock *currentBlock;

    std::queue<Scope*> scopeQueue;
    Scope* currentScope;

    TransformContext(llvm::LLVMContext &llvmContext, llvm::Module &module, llvm::IRBuilder<> &builder,
                     llvm::IRBuilder<> &allocBuilder) : llvmContext(llvmContext), module(module), builder(builder),
                                                        allocBuilder(allocBuilder) {

    }

    llvm::BasicBlock *createBasicBlock(const std::string &name) {
        auto bb = llvm::BasicBlock::Create(llvmContext, name, currentFunction, 0);
        return bb;
    }

    llvm::BasicBlock *createFunction(const std::string &name) {
        llvm::Type *returnType = builder.getVoidTy();
        llvm::FunctionType *functionType = llvm::FunctionType::get(returnType, /* isVarArg */ false);

        /* Create a function declaration for 'fac' */
        currentFunction = llvm::Function::Create(
                functionType,
                llvm::GlobalValue::ExternalLinkage,
                name,
                &module);

        auto entry = createBasicBlock("entry");

        builder.SetInsertPoint(entry);
        allocBuilder.SetInsertPoint(entry);

        return entry;
    }

    void setCurrentBlock(llvm::BasicBlock* bb){
        currentBlock = bb;
        builder.SetInsertPoint(bb);
    }

    void pushScope(Scope* scope){
        scopeQueue.push(scope);
        currentScope = scope;
    }

    void popScope(){
        scopeQueue.pop();
        currentScope = scopeQueue.front();
    }

    llvm::IRBuilder<> &resetAllocBuilder() {
        allocBuilder.SetInsertPoint(allocBuilder.GetInsertBlock(),
                                    allocBuilder.GetInsertBlock()->begin());
        return allocBuilder;
    }

    void dump() {
        //verifyModule(module);
        module.dump();
    }
};
