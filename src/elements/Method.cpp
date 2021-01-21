//
// Created by chris on 21.01.21.
//

#include "Method.h"
#include "../parser/PrettyPrinter.h"
#include "../transform/TransformContext.h"
#include "Block.h"
#include "Declaration.h"

void Method::dump(PrettyPrinter &printer) {
    declaration->dump(printer);
    printer.newLine();
    body->dump(printer);
}

void Method::create(TransformContext &context) {
    auto entry = context.createFunction("main");
    context.setCurrentBlock(entry);
    body->create(context);
}