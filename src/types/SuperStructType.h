//
// Created by chris on 21.01.21.
//

#pragma once

#include <unordered_map>
#include <vector>
#include "Types.h"
#include "../elements/Identifier.h"

class SuperStructType : public SuperType{
public:
    std::unordered_map<const std::string*, int> map;
    std::vector<const SuperType*> types;

    explicit SuperStructType() {

    }

    explicit SuperStructType(bool assignable) : SuperType(assignable) {

    }

    bool equals(const SuperType *other) const override {
        if(auto structType = other->asSuperStructType()){
            return structType == this;
        }
        return false;
    }

    const SuperType* member(const Identifier* identifier) const {
        if(identifier){
            auto pos = map.find(identifier->value);

            if(pos == map.end()){
                return nullptr;
            }

            return types[pos->second];
        }

        return nullptr;
    }

    const SuperStructType* asSuperStructType() const override{
        return this;
    }
};



