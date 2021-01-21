//
// Created by chris on 21.01.21.
//

#include "SuperType.h"
#include "MethodType.h"
#include "PointerType.h"
#include "SuperStructType.h"
#include "SimpleType.h"


const MethodType *SuperType::asMethodType() const {
    return nullptr;
}

const PointerType *SuperType::asPointerType() const {
    return nullptr;
}

const SuperStructType *SuperType::asSuperStructType() const {
    return nullptr;
}

const SimpleType *SuperType::asSimpleType() const {
    return nullptr;
}

bool SuperType::isAssignable() const {
    return assignable;
}