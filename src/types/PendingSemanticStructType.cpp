//
// Created by chris on 21.01.21.
//

#include "PendingSemanticStructType.h"

#include "SemanticType.h"
#include "SemanticStructType.h"
#include "../semantic/Scope.h"

bool PendingSemanticStructType::equals(const SemanticType *other) const {
    auto type = asSemanticStructType();
    if (type == nullptr) {
        return false;
    }
    return type->equals(other);
}

const SemanticStructType *PendingSemanticStructType::asSemanticStructType() const {
    auto desc = scope->getStruct(name);

    if (desc == nullptr) {
        return nullptr;
    }

    return desc->semanticType;
}

bool PendingSemanticStructType::isAssignable() const {
    if (auto superStruct = asSemanticStructType()) {
        return superStruct->assignable;
    }

    return false;
}