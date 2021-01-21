//
// Created by chris on 21.01.21.
//

#include "MethodType.h"
#include "../elements/Call.h"

const SuperType *MethodType::call(const Call *call) const {
    if (call) {
        if (types.size() != call->values.size()) {
            return nullptr;
        }
        int min = std::min(types.size(), call->values.size());

        for (int i = 0; i < min; i++) {
            if (!types[i]->equals(call->values[i]->superType)) {
                return nullptr;
            }
        }

        return subType;
    }

    return nullptr;
}

bool MethodType::equals(const SuperType *other) const {
    if (auto superType = other->asMethodType()) {
        if (types.empty() || superType->types.empty()) {
            return true;
        }

        if (types.size() != superType->types.size()) {
            return false;
        }

        for (unsigned long i = 0; i < superType->types.size(); i++) {
            if (!types[i]->equals(superType->types[i])) {
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