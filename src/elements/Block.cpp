//
// Created by chris on 20.01.21.
//

#include "Block.h"
#include "Util.h"
#include "../parser/PrettyPrinter.h"
#include "../transform/TransformContext.h"

void Block::dump(PrettyPrinter &printer) {
    printer << "{";
    printer.increaseDepth();
    printer.newLine();
    for (auto child : children) {
        printStatement(printer, child);
        printer.newLine();
    }
    printer.decreaseDepth();
    printer << "}";
}

void Block::create(TransformContext &context) {
    auto block = context.createBasicBlock("new-block");
    context.builder.CreateBr(block);
    context.setCurrentBlock(block);
    context.pushScope(scope);
    for (auto child  : children) {
        child->create(context);
    }
    context.popScope();
}