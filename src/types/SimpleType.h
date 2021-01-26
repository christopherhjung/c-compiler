//
// Created by chris on 21.01.21.
//

#pragma once

#include "SuperType.h"
#include "../types/TypeDefines.h"

class SimpleType : public SuperType {
public:
    int id;

    explicit SimpleType(int id) : id(id) {

    }

    explicit SimpleType(int id, bool assignable) : SuperType(assignable), id(id) {

    }

    int getSize() override{
        if(id == TYPE_INT){
            return 4;
        }else{
            return 1;
        }
    }

    bool equals(const SuperType *other) const override {
        if (auto otherSimple = other->asSimpleType()) {
            return id == otherSimple->id;
        }
        return false;
    }

    const SimpleType *asSimpleType() const override {
        return this;
    }
};



