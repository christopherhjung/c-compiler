//
// Created by chris on 21.01.21.
//

#pragma once

#include <vector>
#include "ComplexType.h"
#include "PointerType.h"

class Call;

class MethodType : public ComplexType {
public:
    std::vector<SemanticType *> types;
    std::vector<Location> locations;
    const PointerType* originalType = nullptr;

    explicit MethodType(const SemanticType *subType) : ComplexType(subType) {
        this->originalType = new PointerType(this);
    }

    const SemanticType *call(const Call *call) const;

    bool equals(const SemanticType *other) const override;

    const MethodType *asMethodType() const override;

    const PointerType *packMethodType() const override;

    const MethodType *unpackMethodType() const override;
};



