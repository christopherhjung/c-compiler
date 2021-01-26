//
// Created by chris on 21.01.21.
//

#pragma once

#include "SemanticType.h"
#include "../types/TypeDefines.h"

class SimpleType : public SemanticType {
public:
    int id;

    explicit SimpleType(int id) : id(id) {

    }

    explicit SimpleType(int id, bool assignable) : SemanticType(assignable), id(id) {

    }

    bool equals(const SemanticType *other) const override {
        if (auto otherSimple = other->asSimpleType()) {
            return id == otherSimple->id;
        }
        return false;
    }

    const SimpleType *asSimpleType() const override {
        return this;
    }
};



