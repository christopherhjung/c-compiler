//
// Created by chris on 21.01.21.
//

#pragma once

#include "Elements.h"

class Assignment : public Binary{
public:
    llvm::Value* createRightValue(TransformContext &context) override;

    llvm::Value* create(TransformContext &context) override;

    llvm::BasicBlock* create(TransformContext &context, llvm::BasicBlock* start) override;
};


