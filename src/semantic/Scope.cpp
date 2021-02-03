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
        return reinterpret_cast<llvm::Function*>(current->second.anchor->value);
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

Descriptor<SemanticStructType> *Scope::getStruct(const std::string *str) {
    auto current = structs.find(str);
    if (current != structs.end()) {
        return &current->second;
    } else if (parent != nullptr) {
        return parent->getStruct(str);
    }

    return nullptr;
}

bool Scope::setStruct(const std::string *str, SemanticStructType *type) {
    if (str == nullptr) {
        ERROR(Location());
    }

    auto desc = &structs[str];

    if (desc->defined) {
        if (!desc->semanticType->equals(type)) {
            return false;
        }
    } else {
        desc->semanticType = type;
        desc->defined = true;
    }

    return true;
}

bool Scope::set(const std::string *str, SemanticType *type, bool hasImplementation) {
    if (str == nullptr) {
        ERROR(Location());
    }

    auto desc = &types[str];

    if (desc->defined) {
        bool isMethod = type->asMethodType();
        if (!isMethod) {
            return false;
        } else if (!desc->semanticType->equals(type)) {
            return false;
        } else {
            desc->semanticType = type;
        }
    } else {
        desc->semanticType = type;
        desc->defined = true;
    }

    if (hasImplementation) {
        if (desc->implementation) {
            return false;
        } else {
            desc->implementation = true;
        }
    }

    return true;
}

Descriptor<SemanticType> *Scope::get(const std::string *str) {
    if (types.find(str) != types.end()) {
        return &types[str];
    } else if (parent != nullptr) {
        return parent->get(str);
    } else {
        return &types[str];
    }
}