//
// Created by chris on 20.01.21.
//

#include "Block.h"

void Block::dump(PrettyPrinter& printer){
    printer << "{";
    printer.increaseDepth();
    printer.newLine();
    for(auto child : children){
        printStatement(printer, child);
        printer.newLine();
    }
    printer.decreaseDepth();
    printer << "}";
}

llvm::BasicBlock* Block::create(TransformContext &context, llvm::BasicBlock *start){
    for( auto child  : children ){
        start = child->create(context, start);
    }

    return start;
}