//
// Created by chris on 21.01.21.
//

#pragma once

#include "ComplexType.h"

class PointerType : public ComplexType {
public:
    explicit PointerType(const SemanticType *subType) : ComplexType(subType) {

    }

    explicit PointerType(const SemanticType *subType, bool assignable) : ComplexType(subType, assignable) {

    }

    bool equals(const SemanticType *other) const override;

    const PointerType *asPointerType() const override {
        return this;
    }
};


