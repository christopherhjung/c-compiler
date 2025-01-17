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

    const MethodType *unpackMethodType() const override {
        return inner->unpackMethodType();
    }

    const SemanticType *packMethodType() const override {
        return this;
    }

    const MethodType *asMethodType() const override {
        return inner->asMethodType();
    }

    const PointerType *asPointerType() const override {
        return inner->asPointerType();
    }

    const SemanticStructType *asSemanticStructType() const override {
        return inner->asSemanticStructType();
    }
};


