//
// Created by chris on 21.01.21.
//

#pragma once

#include "../utils/Comparable.h"
#include "../elements/Identifier.h"

class MethodType;
class PointerType;
class SuperStructType;
class SimpleType;

class SuperType : public Comparable<SuperType> {
public:
    bool assignable = false;
    const Identifier *identifier = nullptr;

    SuperType() {};

    SuperType(bool assignable) : assignable(assignable) {

    };

    virtual int getSize(){
        return 4;
    }

    virtual const MethodType *asMethodType() const;

    virtual const PointerType *asPointerType() const;

    virtual const SuperStructType *asSuperStructType() const;

    virtual const SimpleType *asSimpleType() const;

    virtual bool isAssignable() const;
};



