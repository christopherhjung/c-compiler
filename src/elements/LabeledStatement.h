//
// Created by chris on 21.01.21.
//

#pragma once

#include "Elements.h"

class LabeledStatement : public Statement{
public:
    Statement* statement = nullptr;
    const std::string *name = nullptr;

    void dump(PrettyPrinter& printer) override{
        printer.withoutIndent();
        printer << *name << ":";
        printer.newLine();
        printStatement(printer, statement);
    }
};



