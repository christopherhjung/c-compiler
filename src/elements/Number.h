//
// Created by chris on 21.01.21.
//

#pragma once

#include <string>
#include "Expression.h"

class PrettyPrinter;

class Number : public Expression {
public:
    const std::string *value = nullptr;

    void dump(PrettyPrinter &printer) override;

    int getValue();
    llvm::Value *createRightValue(TransformContext &context) override;
};




