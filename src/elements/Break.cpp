//
// Created by chris on 21.01.21.
//

#include "Break.h"
#include "../parser/PrettyPrinter.h"
#include "../transform/TransformContext.h"

void Break::dump(PrettyPrinter &printer) {
    printer << "break";
}

void Break::create(TransformContext &context){
    context.builder.CreateBr(context.whileCondition);
}