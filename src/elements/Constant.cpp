//
// Created by chris on 21.01.21.
//

#include "Constant.h"
#include "../parser/PrettyPrinter.h"

void Constant::dump(PrettyPrinter &printer) {
    printer << *value;
}