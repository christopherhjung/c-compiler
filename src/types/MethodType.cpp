//
// Created by chris on 21.01.21.
//

#include "MethodType.h"
#include "../elements/Call.h"

const SemanticType *MethodType::call(const Call *call) const {
    if (call) {
        if (types.size() != call->values.size()) {
            return nullptr;
        }
        int min = std::min(types.size(), call->values.size());

        for (int i = 0; i < min; i++) {
            if (!types[i]->equals(call->values[i]->getType())) {
                return nullptr;
            }
        }

        return subType;
    }

    return nullptr;
}

bool MethodType::equals(const SemanticType *other) const {
    if (auto semanticType = other->asMethodType()) {
        if (types.empty() || semanticType->types.empty()) {
            return true;
        }

        if (types.size() != semanticType->types.size()) {
            return false;
        }

        for (unsigned long i = 0; i < semanticType->types.size(); i++) {
            if (!types[i]->equals(semanticType->types[i])) {
                return false;
            }
        }

        return true;
    }
    return false;
}

const MethodType *MethodType::asMethodType() const {
    return this;
}

const PointerType *MethodType::packMethodType() const {
    return originalType;
}

const MethodType *MethodType::unpackMethodType() const {
    return this;
}