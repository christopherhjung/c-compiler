//
// Created by chris on 21.01.21.
//

#pragma once

#include "SuperType.h"

class Scope;

class PendingSuperStructType : public SuperType {
public:
    const std::string *name;
    Scope *scope;

    explicit PendingSuperStructType(const std::string *name, Scope *scope) : name(name), scope(scope) {

    }

    bool equals(const SuperType *other) const override;

    const SuperStructType *asSuperStructType() const override;

    bool isAssignable() const override;
};



