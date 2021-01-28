//
// Created by chris on 21.01.21.
//

#include "Unit.h"
#include "../parser/PrettyPrinter.h"
#include "../transform/TransformContext.h"
#include "Declaration.h"
#include "Method.h"
#include "Util.h"

void Unit::dump(PrettyPrinter &printer) {
    bool first = true;
    for (auto child : children) {
        if (first) {
            first = false;
        } else {
            printer.newLine();
            printer.newLine();
        }
        child->dump(printer);
        if (instanceof<Declaration>(child)) {
            printer << ";";
        }
    }
}

void Unit::create(TransformContext &context) {
    context.setMainScope(scope);
    context.pushScope(scope);
    for (auto element : children) {
        if (auto method = dynamic_cast<Method *>(element)) {
            method->create(context);
        } else if (auto decl = dynamic_cast<Declaration *>(element)) {
            decl->create(context);
        }
    }
    context.popScope();
}