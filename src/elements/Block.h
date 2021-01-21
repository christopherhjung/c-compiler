//
// Created by chris on 20.01.21.
//

#pragma once

#include <vector>
#include "Statement.h"

#include "llvm/IR/GlobalValue.h"

class PrettyPrinter;

class TransformContext;

class Scope;

class Block : public Statement {
public:
    Scope *scope;

    std::vector<Statement *> children;

    void dump(PrettyPrinter &printer) override;

    void create(TransformContext &context) override;
};
