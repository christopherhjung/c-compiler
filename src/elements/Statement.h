//
// Created by chris on 21.01.21.
//

#pragma once

#include "Element.h"
#include "llvm/IR/GlobalValue.h"
#include "../transform/TransformException.h"

class PrettyPrinter;

class SuperType;

class TransformContext;

class Statement : public Element {
public:
    const SuperType *superType = nullptr;

    virtual void create(TransformContext &context) {
        TRANSFORM_ERROR();
    }
};



