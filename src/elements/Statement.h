//
// Created by chris on 21.01.21.
//

#pragma once

#include "../transform/TransformException.h"
#include "../types/SuperType.h"

#include "Element.h"
class Statement : public Element{
public:
    const SuperType* superType = nullptr;

    virtual llvm::Value* create(TransformContext &context){
        TRANSFORM_ERROR();
    }

    virtual llvm::BasicBlock* create(TransformContext &context, llvm::BasicBlock* start){
        TRANSFORM_ERROR();
    }
};




