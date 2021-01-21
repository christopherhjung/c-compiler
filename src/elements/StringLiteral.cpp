//
// Created by chris on 21.01.21.
//

#include "StringLiteral.h"
#include "../parser/PrettyPrinter.h"

void StringLiteral::dump(PrettyPrinter &printer) {
    printer << *value;
}