//
// Created by chris on 21.01.21.
//

#pragma once

#include <unordered_map>
#include <string>

#include "Identifier.h"
#include "llvm/IR/GlobalValue.h"

class SemanticType;

class SemanticStructType;
class IdentifierUse;

class Scope {
public:
    std::unordered_map<const std::string *, Identifier*> identifiers;
    std::unordered_map<const std::string *, SemanticStructType*> structs;
    std::unordered_map<const std::string *, int> labels;

    Scope *parent = nullptr;
    const SemanticType *returnType = nullptr;

    Scope() {}

    llvm::Function* getFunction(const std::string* str);

    bool isLabel(const std::string *label);

    void setLabel(const std::string *label);

    const SemanticType *getReturnType();

    SemanticStructType *getStruct(const std::string *str);

    bool structDeclaredInScope(const std::string *str);

    bool setStruct(const std::string *str, SemanticStructType *type);

    bool identifierDeclaredInScope(Identifier *identifier);

    bool set(Identifier *identifier);

    Identifier *get(const std::string *str);
};

