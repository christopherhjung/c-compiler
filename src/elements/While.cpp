//
// Created by chris on 21.01.21.
//

#include "While.h"
#include "../parser/PrettyPrinter.h"
#include "Util.h"
#include "Expression.h"

void While::dump(PrettyPrinter &printer) {
    printer << "while (";
    condition->dump(printer);
    printer << ")";
    printIndentIfNotBlock(printer, body);
}