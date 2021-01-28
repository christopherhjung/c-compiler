//
// Created by chris on 21.01.21.
//

#include "Return.h"
#include "../parser/PrettyPrinter.h"
#include "Expression.h"
#include "../transform/TransformContext.h"
#include "Assignment.h"

void Return::dump(PrettyPrinter &printer) {
    printer << "return";
    if (value != nullptr) {
        printer << " ";
        value->dump(printer);
    }
}

void Return::create(TransformContext &context) {
    if(value == nullptr){
        context.builder.CreateRetVoid();
    }else{
        context.builder.CreateRet(Assignment::ensureAssignment(context, semanticType , value));
    }

    context.setCurrentBlock(context.unreachable);
}