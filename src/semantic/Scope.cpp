//
// Created by chris on 21.01.21.
//

#include "Scope.h"
#include "../lexer/Location.h"
#include "../semantic/SemanticException.h"
#include "../types/SemanticType.h"
#include "../types/SemanticStructType.h"


bool Scope::isLabel(const std::string *label) {
    return labels.find(label) != labels.end();
}

void Scope::setLabel(const std::string *label) {
    labels[label] = 0;
}

llvm::Function* Scope::getFunction(const std::string* str){
    auto current = types.find(str);
    if (current != types.end()) {
        return llvm::dyn_cast<llvm::Function>(current->second->llvmValue);
    } else if (parent != nullptr) {
        return parent->getFunction(str);
    }

    return nullptr;
}

const SemanticType *Scope::getReturnType() {
    if (returnType != nullptr) {
        return returnType;
    } else if (parent != nullptr) {
        return parent->getReturnType();
    }

    return nullptr;
}

bool Scope::structDeclaredInScope(const std::string *str) {
    auto current = structs.find(str);
    return current != structs.end();
}

bool Scope::identifierDeclaredInScope(Identifier *identifier) {
    auto current = types.find(identifier->value);
    return current != types.end();
}

SemanticStructType *Scope::getStruct(const std::string *str) {
    auto current = structs.find(str);
    if (current != structs.end()) {
        return current->second;
    } else if (parent != nullptr) {
        return parent->getStruct(str);
    }

    return nullptr;
}

bool Scope::setStruct(const std::string *str, SemanticStructType *type) {
    if (str == nullptr) {
        ERROR(Location());
    }

    if(structs.find(str) == structs.end()){
        structs[str] = type;
        return true;
    }

    return false;
}

bool Scope::set(Identifier *identifier) {
    if (identifier->value == nullptr) {
        ERROR(Location());
    }

    if(types.find(identifier->value) == types.end()){
        types[identifier->value] = identifier;
        return true;
    }

    return false;
}

Identifier *Scope::get(const std::string *str) {
    if (types.find(str) != types.end()) {
        return types[str];
    } else if (parent != nullptr) {
        return parent->get(str);
    } else {
        return types[str];
    }
}