//
// Created by chris on 21.01.21.
//

#pragma once

#include "../utils/Comparable.h"
#include "../elements/IdentifierUse.h"

class MethodType;
class PointerType;
class SemanticStructType;
class SimpleType;

class SemanticType : public Comparable<SemanticType> {
    bool assignable = false;
public:
    Identifier *identifier = nullptr;

    SemanticType() {};

    SemanticType(bool assignable) : assignable(assignable) {

    };

    virtual const MethodType *asMethodType() const;

    virtual const PointerType *asPointerType() const;

    virtual const SemanticStructType *asSemanticStructType() const;

    virtual const SimpleType *asSimpleType() const;

    virtual const SemanticType *packMethodType() const;

    virtual const MethodType *unpackMethodType() const;

    virtual bool isAssignable() const;
};



