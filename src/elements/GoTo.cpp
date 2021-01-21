//
// Created by chris on 21.01.21.
//

#include "GoTo.h"
#include "../parser/PrettyPrinter.h"
#include "../parser/PrettyPrinter.h"

void GoTo::dump(PrettyPrinter &printer) {
    printer << "goto " << *name;
}