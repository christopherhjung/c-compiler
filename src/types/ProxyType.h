//
// Created by chris on 21.01.21.
//

#pragma once

#include "SemanticType.h"

class ProxyType : public SemanticType {
public:
    const SemanticType *inner;

    explicit ProxyType(const SemanticType *inner, bool assignable) : SemanticType(assignable), inner(inner) {

    }

    bool equals(const SemanticType *other) const override {
        return inner->equals(other);
    }

    const SimpleType *asSimpleType() const override {
        return inner->asSimpleType();
    }

    const MethodType *asMethodType() const override {
        return inner->asMethodType();
    }

    const PointerType *asPointerType() const override {
        return inner->asPointerType();
    }

    const SemanticStructType *asSuperStructType() const override {
        return inner->asSuperStructType();
    }
};


