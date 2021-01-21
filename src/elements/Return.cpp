//
// Created by chris on 21.01.21.
//

#include "Return.h"
#include "../parser/PrettyPrinter.h"
#include "Expression.h"

void Return::dump(PrettyPrinter &printer) {
    printer << "return";
    if (value != nullptr) {
        printer << " ";
        value->dump(printer);
    }
}