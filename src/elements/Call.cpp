//
// Created by chris on 21.01.21.
//

#include "Call.h"

#include "../parser/PrettyPrinter.h"

void Call::dump(PrettyPrinter &printer) {
    printer << "(";
    target->dump(printer);
    printer << "(";
    bool first = true;
    for (auto val : values) {
        if (first) {
            first = false;
        } else {
            printer << ", ";
        }
        val->dump(printer);
    }
    printer << ")";
    printer << ")";
}