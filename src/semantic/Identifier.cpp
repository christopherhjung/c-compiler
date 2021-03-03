//
// Created by chris on 04.02.21.
//

#include "Identifier.h"

#include "../parser/PrettyPrinter.h"

void Identifier::dump(PrettyPrinter &printer) {
    printer << *value;
}