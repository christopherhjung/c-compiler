//
// Created by chris on 21.01.21.
//

#include "LabeledStatement.h"
#include "../parser/PrettyPrinter.h"
#include "Util.h"

void LabeledStatement::dump(PrettyPrinter &printer) {
    printer.withoutIndent();
    printer << *name << ":";
    printer.newLine();
    printStatement(printer, statement);
}