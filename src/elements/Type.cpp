//
// Created by chris on 21.01.21.
//

#include "Type.h"
#include "../parser/PrettyPrinter.h"
#include "../types/TypeDefines.h"

void Type::dump(PrettyPrinter &printer) {
    if (type == TYPE_INT) {
        printer << "int";
    } else if (type == TYPE_CHAR) {
        printer << "char";
    } else if (type == TYPE_VOID) {
        printer << "void";
    }
}