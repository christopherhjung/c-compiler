//
// Created by chris on 21.01.21.
//

#pragma once

#include "ComplexType.h"

class PointerType : public ComplexType {
public:
    explicit PointerType(const SuperType *subType) : ComplexType(subType) {

    }

    explicit PointerType(const SuperType *subType, bool assignable) : ComplexType(subType, assignable) {

    }

    bool equals(const SuperType *other) const override;

    const PointerType *asPointerType() const override {
        return this;
    }
};


