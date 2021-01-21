//
// Created by chris on 21.01.21.
//

#pragma once

#include "SuperType.h"

class ComplexType : public SuperType {
public:
    const SuperType *subType = nullptr;

    explicit ComplexType(const SuperType *subType) : subType(subType) {

    }

    explicit ComplexType(const SuperType *subType, bool assignable) : SuperType(assignable), subType(subType) {

    }
};



