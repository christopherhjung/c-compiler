//
// Created by chris on 20.01.21.
//


#include "TransformContext.h"


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


llvm::BasicBlock *TransformContext::createBasicBlock(const std::string &name) {
    auto bb = llvm::BasicBlock::Create(llvmContext, name, currentFunction, 0);
    return bb;
}

llvm::Value *TransformContext::getInt32(const std::string *str) {
    return builder.getInt32(std::stoi(*str));
}

llvm::Type *TransformContext::getType(const SemanticType *type) {
    if (auto simpleType = type->asSimpleType()) {
        if (simpleType->equals(IntType)) {
            return builder.getInt32Ty();
        } else if (simpleType->equals(CharType)) {
            return builder.getInt8Ty();
        } else {
            return builder.getVoidTy();
        }
    } else if (auto pointerType = type->asPointerType()) {
        if(pointerType->equals(VoidPointerType)){
            return builder.getInt8PtrTy();
        }else{
            return llvm::PointerType::getUnqual(getType(pointerType->subType));
        }
    } else if (auto structType = type->asSuperStructType()) {
        auto find = typeLookup.find(structType);

        if (find != typeLookup.end()) {
            return find->second;
        }

        auto llvmStructType = llvm::StructType::create(llvmContext, "struct.S");
        typeLookup[structType] = llvmStructType;

        std::vector<llvm::Type *> types;
        for (auto type : structType->types) {
            types.push_back(getType(type));
        }

        llvmStructType->setBody(types);
        return llvmStructType;
    }else if(auto methodType = type->asMethodType()){
        auto returnType = getType(methodType->subType);
        std::vector<llvm::Type*> paramTypes;
        for(auto type : methodType->types){
            if(!type->equals(VoidType)){
                paramTypes.push_back(getType(type));
            }
        }

        return llvm::FunctionType::get(returnType, paramTypes, false);
    }

    return nullptr;
}

llvm::BasicBlock *TransformContext::getJumpTarget(const std::string *target) {
    auto find = jumps.find(target);
    if (find != jumps.end()) {
        return find->second;
    } else {
        return jumps[target] = createBasicBlock("label-" + *target);
    }
}

void TransformContext::createFunctionDecl(const std::string &name, llvm::Type *returnType,
                                          std::vector<llvm::Type *> &paramTypes) {
    jumps.clear();
    llvm::FunctionType *functionType = llvm::FunctionType::get(returnType, paramTypes, /* isVarArg */ false);

    /* Create a function declaration for 'fac' */
    currentFunction = llvm::Function::Create(
            functionType,
            llvm::GlobalValue::ExternalLinkage,
            name,
            &module);
}

llvm::BasicBlock *TransformContext::createFunction(const std::string &name, llvm::Type *returnType,
                                 std::vector<llvm::Type *> &paramTypes) {
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

void TransformContext::setCurrentBlock(llvm::BasicBlock *bb) {
    currentBlock = bb;
    builder.SetInsertPoint(bb);
}

void TransformContext::setMainScope(Scope *scope) {
    mainScope = scope;
}

void TransformContext::pushScope(Scope *scope) {
    scopeQueue.push(scope);
    currentScope = scope;
}

void TransformContext::popScope() {
    scopeQueue.pop();
    currentScope = scopeQueue.front();
}

llvm::IRBuilder<> &TransformContext::resetAllocBuilder() {
    allocBuilder.SetInsertPoint(allocBuilder.GetInsertBlock(),
                                allocBuilder.GetInsertBlock()->begin());
    return allocBuilder;
}

void TransformContext::println() {
    verifyModule(module);
    module.dump();
}

void TransformContext::dump(const std::string &filename) {
    std::error_code EC;
    llvm::raw_fd_ostream stream(filename, EC, llvm::sys::fs::OpenFlags::F_Text);
    module.print(stream, nullptr);
}
