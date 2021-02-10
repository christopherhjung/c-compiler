//
// Created by chris on 21.01.21.
//

#pragma once

#include <string>
#include "Expression.h"

class PrettyPrinter;

class StringLiteral : public Expression {
public:
    const std::string *value = nullptr;

    void dump(PrettyPrinter &printer) override;

    llvm::Value *createRightValue(TransformContext &context) override;

    int getSize(TransformContext &context) override;
};



