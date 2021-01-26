//
// Created by chris on 21.01.21.
//

#include "PendingSuperStructType.h"

#include "SemanticType.h"
#include "SemanticStructType.h"
#include "../semantic/Scope.h"

bool PendingSuperStructType::equals(const SemanticType *other) const {
    auto type = asSuperStructType();
    if (type == nullptr) {
        return false;
    }
    return type->equals(other);
}

const SemanticStructType *PendingSuperStructType::asSuperStructType() const {
    auto desc = scope->getStruct(name);

    if (desc == nullptr) {
        return nullptr;
    }

    return desc->semanticType;
}

bool PendingSuperStructType::isAssignable() const {
    if (auto superStruct = asSuperStructType()) {
        return superStruct->assignable;
    }

    return false;
}