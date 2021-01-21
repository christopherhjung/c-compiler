//
// Created by chris on 21.01.21.
//

#pragma once

#include "ComplexType.h"
#include <vector>
#include "../lexer/Location.h"
#include "../elements/Call.h"
#include "Types.h"

class MethodType : public ComplexType{
public:
    std::vector<SuperType*> types;
    std::vector<Location> locations;

    explicit MethodType(const SuperType* subType) : ComplexType(subType) {

    }

    const SuperType* call(const Call* call) const {
        if(call){
            if( types.size() != call->values.size() ){
                return nullptr;
            }
            int min = std::min(types.size() , call->values.size());

            for( int i = 0 ; i < min ; i++ ){
                if( !types[i]->equals(call->values[i]->superType) ){
                    return nullptr;
                }
            }

            return subType;
        }

        return nullptr;
    }

    bool equals(const SuperType *other) const override {
        if( auto superType =other->asMethodType() ){
            if(types.empty() || superType->types.empty()){
                return true;
            }

            if( types.size() != superType->types.size() ){
                return false;
            }

            for( unsigned long i = 0 ; i < superType->types.size() ; i++ ){
                if( !types[i]->equals(superType->types[i]) ){
                    return false;
                }
            }

            return true;
        }
        return false;
    }

    const MethodType* asMethodType() const override{
        return this;
    }
};



