//
// Created by chris on 21.01.21.
//

#pragma once

#include "SemanticType.h"
#include "../elements/StructType.h"

class Scope;

class PendingSemanticStructType : public SemanticType {
public:
    const std::string* name;
    Scope *scope;
    Location location;

    PendingSemanticStructType(const std::string* name, Location* location, Scope *scope) : name(name), location(*location), scope(scope) {

    }

    bool equals(const SemanticType *other) const override;

    const SemanticStructType *asSemanticStructType() const override;

    bool isAssignable() const override;
};



