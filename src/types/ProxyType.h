//
// Created by chris on 21.01.21.
//

#pragma once

#include "SuperType.h"

class ProxyType : public SuperType {
public:
    const SuperType *inner;

    explicit ProxyType(const SuperType *inner, bool assignable) : SuperType(assignable), inner(inner) {

    }

    int getSize() override{
        return const_cast<SuperType*>(inner)->getSize();
    }

    bool equals(const SuperType *other) const override {
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

    const SuperStructType *asSuperStructType() const override {
        return inner->asSuperStructType();
    }
};


