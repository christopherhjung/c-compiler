//
// Created by chris on 21.01.21.
//
#pragma once

#include <string>
#include "DirectDeclarator.h"
#include "Expression.h"
#include "../semantic/Identifier.h"

class PrettyPrinter;

class IdentifierUse : public Expression {
public:
    Identifier* identifier = nullptr;
    const std::string *value = nullptr;

    IdentifierUse() {}

    IdentifierUse(const std::string *value) : value(value) {}

    void dump(PrettyPrinter &printer) override;

    bool operator==(const IdentifierUse &rhs) const {
        return value == rhs.value;
    }

    bool operator!=(const IdentifierUse &rhs) const {
        return !(rhs == *this);
    }

    llvm::Value *createRightValue(TransformContext &context) override;

    llvm::Value *createLeftValue(TransformContext &context) override;


    const SemanticType *getType() override;
};



