//
// Created by chris on 21.01.21.
//

#pragma once

#include "SemanticType.h"

class ComplexType : public SemanticType {
public:
    const SemanticType *subType = nullptr;

    explicit ComplexType(const SemanticType *subType) : subType(subType) {

    }

    explicit ComplexType(const SemanticType *subType, bool assignable) : SemanticType(assignable), subType(subType) {

    }
};



