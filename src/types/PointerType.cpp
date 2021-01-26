//
// Created by chris on 21.01.21.
//

#include "PointerType.h"
#include "ComplexType.h"
#include "SemanticType.h"

bool PointerType::equals(const SemanticType *other) const {
    if (auto simpleOther = other->asPointerType()) {
        return subType->equals(simpleOther->subType);
    }
    return false;
}
