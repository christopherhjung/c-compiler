//
// Created by chris on 21.01.21.
//

#include "Scope.h"
#include "../lexer/Location.h"
#include "../semantic/SemanticException.h"
#include "../types/SuperType.h"
#include "../types/SuperStructType.h"

bool Scope::isLabel(const std::string *label) {
    return labels.find(label) != labels.end();
}

void Scope::setLabel(const std::string *label) {
    labels[label] = 0;
}

const SuperType *Scope::getReturnType() {
    if (returnType != nullptr) {
        return returnType;
    } else if (parent != nullptr) {
        return parent->getReturnType();
    }

    return nullptr;
}

Descriptor<SuperStructType> *Scope::getStruct(const std::string *str) {
    auto current = structs.find(str);
    if (current != structs.end()) {
        return &current->second;
    } else if (parent != nullptr) {
        return parent->getStruct(str);
    }

    return nullptr;
}

bool Scope::setStruct(const std::string *str, SuperStructType *type) {
    if (str == nullptr) {
        ERROR(Location());
    }

    auto desc = &structs[str];

    if (desc->defined) {
        if (!desc->superType->equals(type)) {
            return false;
        }
    } else {
        desc->superType = type;
        desc->defined = true;
    }

    return true;
}

bool Scope::set(const std::string *str, SuperType *type, bool hasImplementation) {
    if (str == nullptr) {
        ERROR(Location());
    }

    auto desc = &types[str];

    if (desc->defined) {
        bool isMethod = type->asMethodType();
        if (!isMethod) {
            return false;
        } else if (!desc->superType->equals(type)) {
            return false;
        } else {
            desc->superType = type;
        }
    } else {
        desc->superType = type;
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

Descriptor<SuperType> *Scope::get(const std::string *str) {
    if (types.find(str) != types.end()) {
        return &types[str];
    } else if (parent != nullptr) {
        return parent->get(str);
    } else {
        return &types[str];
    }
}