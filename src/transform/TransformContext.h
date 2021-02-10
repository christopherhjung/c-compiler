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
public:

    llvm::LLVMContext &llvmContext;
    llvm::Module &module;
    llvm::IRBuilder<> &builder;
    llvm::IRBuilder<> &allocBuilder;
    llvm::DataLayout layout;

    llvm::Function *currentFunction = nullptr;
    llvm::BasicBlock *currentBlock = nullptr;

    llvm::BasicBlock *whileCondition = nullptr;
    llvm::BasicBlock *whileEnd = nullptr;

    llvm::BasicBlock *unreachable = nullptr;

    std::queue<Scope *> scopeQueue;
    Scope *mainScope = nullptr;
    Scope *functionScope = nullptr;
    Scope *currentScope = nullptr;

    std::unordered_map<const SemanticType*, llvm::Type*> typeLookup;

    std::unordered_map<const std::string*, llvm::BasicBlock*> jumps;


    TransformContext(llvm::LLVMContext &llvmContext, llvm::Module &module, llvm::IRBuilder<> &builder,
                     llvm::IRBuilder<> &allocBuilder) : llvmContext(llvmContext), module(module), builder(builder),
                                                        allocBuilder(allocBuilder), layout(&module) {

    }

    llvm::BasicBlock *createBasicBlock(const std::string &name);
    llvm::BasicBlock *createBasicBlock(const std::string &name, llvm::BasicBlock *bb);

    llvm::Value *getInt32(const std::string *str);

    llvm::Type *getType(const SemanticType *type);
    llvm::Type *getSimpleType(int simpleTypeId);
    llvm::Type *getGreatestType(const SemanticType *left, const SemanticType *right);

    llvm::BasicBlock *getJumpTarget(const std::string *target);

    void createFunctionDecl(const std::string &name, llvm::Type *returnType, std::vector<llvm::Type *> &paramTypes);

    llvm::BasicBlock *
    createFunction(const std::string &name, llvm::Type *returnType, std::vector<llvm::Type *> &paramTypes);

    void setCurrentBlock(llvm::BasicBlock *bb) ;

    void setMainScope(Scope* scope);

    void pushScope(Scope *scope);

    void popScope() ;

    llvm::IRBuilder<> &resetAllocBuilder();

    //void println() ;
    void dump(const std::string &filename);
};
