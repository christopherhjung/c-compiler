//
// Created by chris on 21.01.21.
//

#include "PendingSuperStructType.h"

#include "SuperType.h"
#include "SuperStructType.h"
#include "../semantic/Scope.h"

bool PendingSuperStructType::equals(const SuperType *other) const {
    auto type = asSuperStructType();
    if (type == nullptr) {
        return false;
    }
    return type->equals(other);
}

const SuperStructType *PendingSuperStructType::asSuperStructType() const {
    auto desc = scope->getStruct(name);

    if (desc == nullptr) {
        return nullptr;
    }

    return desc->superType;
}

bool PendingSuperStructType::isAssignable() const {
    if (auto superStruct = asSuperStructType()) {
        return superStruct->assignable;
    }

    return false;
}