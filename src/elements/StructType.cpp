//
// Created by chris on 21.01.21.
//

#include "StructType.h"
#include "../parser/PrettyPrinter.h"
#include "Declaration.h"

void StructType::dump(PrettyPrinter &printer) {
    printer << "struct";

    if (name != nullptr) {
        printer << " " << *name;
    }

    if (!declarations.empty()) {
        printer.newLine();
        printer << "{";
        printer.newLine();
        printer.increaseDepth();
        for (auto decl : declarations) {
            decl->dump(printer);
            printer << ";";
            printer.newLine();
        }
        printer.decreaseDepth();
        printer << "}";
    }
}