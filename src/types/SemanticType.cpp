//
// Created by chris on 21.01.21.
//

#include "SemanticType.h"
#include "MethodType.h"
#include "PointerType.h"
#include "SemanticStructType.h"
#include "SimpleType.h"


const MethodType *SemanticType::asMethodType() const {
    return nullptr;
}

const PointerType *SemanticType::asPointerType() const {
    return nullptr;
}

const SemanticStructType *SemanticType::asSemanticStructType() const {
    return nullptr;
}

const SimpleType *SemanticType::asSimpleType() const {
    return nullptr;
}

bool SemanticType::isAssignable() const {
    return assignable;
}