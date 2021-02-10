//
// Created by chris on 21.01.21.
//

#pragma once


#include "Expression.h"
#include <string>

class PrettyPrinter;

class Constant : public Expression {
public:
    const std::string *value = nullptr;

    void dump(PrettyPrinter &printer) override;

    llvm::Value *createRightValue(TransformContext &context) override;

    llvm::Value *getSize(TransformContext &context) override;
};



