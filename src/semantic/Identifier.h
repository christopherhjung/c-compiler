//
// Created by chris on 04.02.21.
//

#pragma once

#include "llvm/IR/Module.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/GlobalValue.h"
#include "llvm/IR/Verifier.h"
#include "llvm/Support/Signals.h"
#include "llvm/Support/SystemUtils.h"
#include "llvm/Support/PrettyStackTrace.h"

#include "../elements/DirectDeclarator.h"
#include "../lexer/Location.h"

class SemanticType;

class Identifier : public DirectDeclarator{
public:
    bool defined = false;
    const std::string *value;
    Location location;
    SemanticType *semanticType;
    llvm::Value* llvmValue = nullptr;

    bool operator==(const Identifier &rhs) const {
        return value == rhs.value;
    }

    bool operator!=(const Identifier &rhs) const {
        return !(rhs == *this);
    }

    void dump(PrettyPrinter &printer) override;

    SemanticType *getType(){
        return semanticType;
    }

    void setType(SemanticType *type){
        semanticType = type;
    }
};