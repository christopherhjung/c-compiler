//
// Created by chris on 20.01.21.
//


#include "TransformContext.h"


#include "llvm/Support/FileSystem.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/GlobalValue.h"
#include "llvm/IR/Verifier.h"
#include "llvm/Support/Signals.h"

#include <queue>


llvm::BasicBlock *TransformContext::createBasicBlock(const std::string &name) {
    return createBasicBlock(name, unreachable);
}

llvm::BasicBlock *TransformContext::createBasicBlock(const std::string &name, llvm::BasicBlock * bb) {
    return llvm::BasicBlock::Create(llvmContext, name, currentFunction, bb);
}

llvm::Value *TransformContext::getInt32(const std::string *str) {
    return builder.getInt32(std::stoi(*str));
}


llvm::Type *TransformContext::getSimpleType(int simpleTypeId){
    if (simpleTypeId == TYPE_INT) {
        return builder.getInt32Ty();
    } else if (simpleTypeId == TYPE_CHAR) {
        return builder.getInt8Ty();
    } else if (simpleTypeId == TYPE_VOID){
        return builder.getVoidTy();
    }

    TRANSFORM_ERROR();
}


llvm::Type *TransformContext::getGreatestType(const SemanticType *left, const SemanticType *right){
    auto leftSimple = left->asSimpleType();
    auto rightSimple = right->asSimpleType();

    if(leftSimple && rightSimple){
        int type = std::min(leftSimple->id, rightSimple->id);
        return getSimpleType(type);
    }

    if(left->asPointerType() || right->asPointerType()){
        return builder.getInt64Ty();
    }

    if(left->equals(right)){
        return getType(left);
    }

    TRANSFORM_ERROR();
}


llvm::Type *TransformContext::getType(const SemanticType *type) {
    if (auto simpleType = type->asSimpleType()) {
        return getSimpleType(simpleType->id);
    } else if (auto pointerType = type->asPointerType()) {
        if(pointerType->equals(VoidPointerType)){
            return builder.getInt8PtrTy();
        }else{
            return llvm::PointerType::getUnqual(getType(pointerType->subType));
        }
    } else if (auto structType = type->asSemanticStructType()) {
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
    auto scope = scopeQueue.front();
    //currentScope = currentScope->parent;
    currentScope = scope;
    /*
    if(scope != currentScope){
        int a = 9;
    }*/
}

llvm::IRBuilder<> &TransformContext::resetAllocBuilder() {
    allocBuilder.SetInsertPoint(allocBuilder.GetInsertBlock(),
                                allocBuilder.GetInsertBlock()->begin());
    return allocBuilder;
}
/*
void TransformContext::println() {
    verifyModule(module);
    module.dump();
}*/

void TransformContext::dump(const std::string &filename) {
    bool correct = verifyModule(module);

    if(!correct){
        //TRANSFORM_ERROR_MSG("Not valid");
    }

    std::error_code EC;
    llvm::raw_fd_ostream stream(filename, EC, llvm::sys::fs::OpenFlags::F_Text);
    module.print(stream, nullptr);
}
