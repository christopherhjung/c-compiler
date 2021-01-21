//
// Created by chris on 21.01.21.
//

#include "ParameterDirectDeclarator.h"
#include "../parser/PrettyPrinter.h"
#include "ParameterTypeList.h"

void ParameterDirectDeclarator::dump(PrettyPrinter &printer) {
    printer << "(";
    if (directDeclarator != nullptr) {
        directDeclarator->dump(printer);
    }

    if (parameterTypeList != nullptr) {
        printer << "(";
        parameterTypeList->dump(printer);
        printer << ")";
    }
    printer << ")";
}