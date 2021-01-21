//
// Created by chris on 21.01.21.
//

#include "Select.h"
#include "../parser/PrettyPrinter.h"

void Select::dump(PrettyPrinter &printer) {
    printer << "(";
    target->dump(printer);
    printer << "[";
    index->dump(printer);
    printer << "]";
    printer << ")";
}