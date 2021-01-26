//
// Created by chris on 21.01.21.
//

#pragma once

#include <vector>
#include "ComplexType.h"

class Call;

class MethodType : public ComplexType {
public:
    std::vector<SemanticType *> types;
    std::vector<Location> locations;

    explicit MethodType(const SemanticType *subType) : ComplexType(subType) {

    }

    const SemanticType *call(const Call *call) const;

    bool equals(const SemanticType *other) const override;

    const MethodType *asMethodType() const override;
};



