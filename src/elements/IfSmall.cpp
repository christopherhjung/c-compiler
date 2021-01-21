//
// Created by chris on 21.01.21.
//

#include "IfSmall.h"
#include "../parser/PrettyPrinter.h"

void IfSmall::dump(PrettyPrinter &printer) {
    printer << "(";
    condition->dump(printer);
    printer << " ? ";
    left->dump(printer);
    printer << " : ";
    right->dump(printer);
    printer << ")";
}