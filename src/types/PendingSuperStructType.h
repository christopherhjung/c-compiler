//
// Created by chris on 21.01.21.
//

#pragma once

#include "SemanticType.h"
#include "SemanticStructType.h"

class Scope;

class PendingSuperStructType : public SemanticType {
public:
    const std::string *name;
    Scope *scope;

    explicit PendingSuperStructType(const std::string *name, Scope *scope) : name(name), scope(scope) {

    }

    bool equals(const SemanticType *other) const override;

    const SemanticStructType *asSuperStructType() const override;

    bool isAssignable() const override;
};



