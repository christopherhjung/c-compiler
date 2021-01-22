//
// Created by chris on 21.01.21.
//

#include "GoTo.h"
#include "../parser/PrettyPrinter.h"
#include "../parser/PrettyPrinter.h"
#include "../transform/TransformContext.h"

void GoTo::dump(PrettyPrinter &printer) {
    printer << "goto " << *name;
}

void GoTo::create(TransformContext &context){
    context.builder.CreateBr(context.getJumpTarget(name));
    context.setCurrentBlock(context.createBasicBlock("after-goto"));
}