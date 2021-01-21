//
// Created by chris on 20.01.21.
//

#pragma once

#include <vector>

#include "Elements.h"
#include "../parser/PrettyPrinter.h"
#include "../transform/TransformContext.h"

class Block : public Statement{
public:
    std::vector<Statement*> children;
    void dump(PrettyPrinter& printer) override;
    llvm::BasicBlock* create(TransformContext &context, llvm::BasicBlock *start) override;
};
