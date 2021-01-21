//
// Created by chris on 21.01.21.
//

#pragma once

#include <vector>
#include "ComplexType.h"

class Call;

class MethodType : public ComplexType {
public:
    std::vector<SuperType *> types;
    std::vector<Location> locations;

    explicit MethodType(const SuperType *subType) : ComplexType(subType) {

    }

    const SuperType *call(const Call *call) const;

    bool equals(const SuperType *other) const override;

    const MethodType *asMethodType() const override;
};



