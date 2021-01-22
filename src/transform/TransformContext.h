#pragma once

#include "llvm/Support/FileSystem.h"
#include "llvm/Support/raw_ostream.h"

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
#include "../types/Types.h"
#include "../lexer/CatchingLexerProxy.h"

#include "../common.h"

class TransformContext {
    llvm::IRBuilder<> &allocBuilder;
public:
    llvm::LLVMContext &llvmContext;
    llvm::Module &module;
    llvm::IRBuilder<> &builder;

    llvm::Function *currentFunction;
    llvm::BasicBlock *currentBlock;

    llvm::BasicBlock *whileCondition;
    llvm::BasicBlock *whileEnd;

    std::queue<Scope *> scopeQueue;
    Scope *mainScope;
    Scope *functionScope;
    Scope *currentScope;

    std::unordered_map<const std::string*, llvm::BasicBlock*> jumps;


    TransformContext(llvm::LLVMContext &llvmContext, llvm::Module &module, llvm::IRBuilder<> &builder,
                     llvm::IRBuilder<> &allocBuilder) : llvmContext(llvmContext), module(module), builder(builder),
                                                        allocBuilder(allocBuilder) {
    }

    void init() {
#ifdef DEBUG
        const std::string printfName = "printf";
        std::vector<llvm::Type *> argumentTypes;
        argumentTypes.push_back(builder.getInt8PtrTy());
        llvm::FunctionType *funcPrintfType = llvm::FunctionType::get(builder.getInt32Ty(), argumentTypes, true);

        llvm::Function *funcPrintf = llvm::Function::Create(
                funcPrintfType,
                llvm::GlobalValue::ExternalLinkage,
                printfName,
                &module);


        mainScope->functions[lookupSymbol(printfName)] = funcPrintf;
#endif
    }

    llvm::BasicBlock *createBasicBlock(const std::string &name) {
        auto bb = llvm::BasicBlock::Create(llvmContext, name, currentFunction, 0);
        return bb;
    }

    llvm::Value *getInt32(const std::string *str) {
        return builder.getInt32(std::stoi(*str));
    }

    llvm::Type *getType(const SuperType *type) {
        if (auto simpleType = type->asSimpleType()) {
            if (simpleType->equals(IntType)) {
                return builder.getInt32Ty();
            } else {
                return builder.getInt8Ty();
            }
        } else if (auto pointerType = type->asPointerType()) {
            return llvm::PointerType::getUnqual(getType(pointerType->subType));
        }

        return nullptr;
    }

    llvm::BasicBlock *getJumpTarget(const std::string *target){
        auto find = jumps.find(target);
        if(find != jumps.end()){
            return find->second;
        }else{
            return jumps[target] = createBasicBlock(*target);
        }
    }

    llvm::BasicBlock *
    createFunction(const std::string &name, llvm::Type *returnType, std::vector<llvm::Type *> &paramTypes) {
        jumps.clear();
        llvm::FunctionType *functionType = llvm::FunctionType::get(returnType, paramTypes, /* isVarArg */ false);

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

    void setCurrentBlock(llvm::BasicBlock *bb) {
        currentBlock = bb;
        builder.SetInsertPoint(bb);
    }

    void setMainScope(Scope* scope){
        mainScope = scope;
    }

    void pushScope(Scope *scope) {
        scopeQueue.push(scope);
        currentScope = scope;
    }

    void popScope() {
        scopeQueue.pop();
        currentScope = scopeQueue.front();
    }

    llvm::IRBuilder<> &resetAllocBuilder() {
        allocBuilder.SetInsertPoint(allocBuilder.GetInsertBlock(),
                                    allocBuilder.GetInsertBlock()->begin());
        return allocBuilder;
    }

    void println() {
        verifyModule(module);
        module.dump();
    }

    void dump(const std::string &filename){
        std::error_code EC;
        llvm::raw_fd_ostream stream(filename, EC, llvm::sys::fs::OpenFlags::F_Text);
        module.print(stream, nullptr);
    }
};
