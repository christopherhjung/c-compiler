//
// Created by chris on 21.01.21.
//

#include "Unary.h"
#include "../parser/PrettyPrinter.h"
#include "Operator.h"

void Unary::dump(PrettyPrinter &printer) {
    printer << "(";
    op->dump(printer);
    value->dump(printer);
    printer << ")";
}