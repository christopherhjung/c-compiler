//
// Created by chris on 21.01.21.
//

#pragma once

#include "Element.h"
#include "llvm/IR/GlobalValue.h"
#include "../transform/TransformException.h"

class PrettyPrinter;

class SemanticType;

class TransformContext;

class Statement : public Element {
public:
    const SemanticType *semanticType = nullptr;

    virtual void create(TransformContext &context) {
        TRANSFORM_ERROR();
    }
};




