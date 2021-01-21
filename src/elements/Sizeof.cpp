//
// Created by chris on 21.01.21.
//

#include "Sizeof.h"
#include "../parser/PrettyPrinter.h"
#include "Type.h"
#include "Declarator.h"

void Sizeof::dump(PrettyPrinter &printer) {
    printer << "(";
    printer << "sizeof(";
    type->dump(printer);
    if (declarator != nullptr) {
        declarator->dump(printer);
    }
    printer << ")";
    printer << ")";
}