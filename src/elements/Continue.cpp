//
// Created by chris on 21.01.21.
//

#include "Continue.h"
#include "../parser/PrettyPrinter.h"
#include "../transform/TransformContext.h"

void Continue::dump(PrettyPrinter &printer) {
    printer << "continue";
}

void Continue::create(TransformContext &context){
    context.builder.CreateBr(context.whileCondition);
    context.setCurrentBlock(context.unreachable);
}