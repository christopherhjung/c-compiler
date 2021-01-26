//
// Created by chris on 21.01.21.
//

#pragma once

#include "../utils/Comparable.h"
#include "../elements/Identifier.h"

class MethodType;
class PointerType;
class SemanticStructType;
class SimpleType;

class SemanticType : public Comparable<SemanticType> {
public:
    bool assignable = false;
    const Identifier *identifier = nullptr;

    SemanticType() {};

    SemanticType(bool assignable) : assignable(assignable) {

    };

    virtual const MethodType *asMethodType() const;

    virtual const PointerType *asPointerType() const;

    virtual const SemanticStructType *asSuperStructType() const;

    virtual const SimpleType *asSimpleType() const;

    virtual bool isAssignable() const;
};



