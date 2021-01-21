//
// Created by chris on 21.01.21.
//

#include "ParameterTypeList.h"
#include "../parser/PrettyPrinter.h"
#include "Declaration.h"

void ParameterTypeList::dump(PrettyPrinter &printer) {
    bool first = true;
    for (auto decl : declarations) {
        if (first) {
            first = false;
        } else {
            printer << ", ";
        }

        decl->dump(printer);
    }
}