//
// Created by chris on 21.01.21.
//

#pragma once

#include "Elements.h"

class While : public Statement{
public:
    Expression* condition = nullptr;
    Statement* body = nullptr;

    void dump(PrettyPrinter& printer) override{
        printer << "while (";
        condition->dump(printer);
        printer << ")";
        printIndentIfNotBlock(printer, body);
    }
};


