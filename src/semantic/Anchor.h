//
// Created by chris on 04.02.21.
//

#pragma once

#include "llvm/IR/Module.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/GlobalValue.h"
#include "llvm/IR/Verifier.h"
#include "llvm/Support/Signals.h"
#include "llvm/Support/SystemUtils.h"
#include "llvm/Support/PrettyStackTrace.h"

class Anchor{
public:
    llvm::Value* value = nullptr;
};