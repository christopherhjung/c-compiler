//
// Created by chris on 21.01.21.
//

#pragma once

#include <unordered_map>
#include <string>

#include "Anchor.h"
#include "llvm/IR/GlobalValue.h"

class SemanticType;

class SemanticStructType;



template<class T>
class Descriptor {
public:
    T *semanticType = nullptr;
    bool implementation = false;
    bool defined = false;
    Anchor* anchor = new Anchor();
    //llvm::Function* function;

    Descriptor() {

    }

    Descriptor(const Descriptor *other) : semanticType(other->semanticType), implementation(other->implementation),
                                          defined(other->defined) {

    }

    Descriptor(SemanticType *semanticType) : semanticType(semanticType) {

    }
};

class Scope {
public:
    std::unordered_map<const std::string *, Descriptor<SemanticType>> types;
    std::unordered_map<const std::string *, Descriptor<SemanticStructType>> structs;
    std::unordered_map<const std::string *, int> labels;
    //std::unordered_map<const std::string *, llvm::Function*> functions;
    Scope *parent = nullptr;
    const SemanticType *returnType = nullptr;

    Scope() {}

    llvm::Function* getFunction(const std::string* str);

    bool isLabel(const std::string *label);

    void setLabel(const std::string *label);

    const SemanticType *getReturnType();

    Descriptor<SemanticStructType> *getStruct(const std::string *str);

    bool structDeclaredInScope(const std::string *str);

    bool setStruct(const std::string *str, SemanticStructType *type);

    bool set(const std::string *str, SemanticType *type, bool hasImplementation);

    Descriptor<SemanticType> *get(const std::string *str);
};

