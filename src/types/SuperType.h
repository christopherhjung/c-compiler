//
// Created by chris on 21.01.21.
//

#pragma once

#include "../elements/Identifier.h"
#include "Types.h"
#include "../utils/Comparable.h"

class SuperType : public Comparable<SuperType>{
public:
    bool assignable = false;
    const Identifier* identifier = nullptr;

    SuperType(){};
    SuperType(bool assignable) : assignable(assignable){

    };

    virtual const MethodType* asMethodType() const;
    virtual const PointerType* asPointerType() const;
    virtual const SuperStructType* asSuperStructType() const;
    virtual const SimpleType* asSimpleType() const;
    virtual bool isAssignable() const;
};



