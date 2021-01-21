//
// Created by chris on 21.01.21.
//

#pragma once

#include "JumpingCompare.h"

class PrettyPrinter;

class LogicalOr : public JumpingCompare {
public:
    void createConditionBranch(TransformContext &context,llvm::BasicBlock *trueBlock,
                               llvm::BasicBlock *falseBlock) override;

};



