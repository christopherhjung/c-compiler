//
// Created by chris on 21.01.21.
//

#pragma once

#include "Elements.h"

class Sizeof : public Expression{
public:
    Type* type = nullptr;
    Declarator *declarator = nullptr;

    void dump(PrettyPrinter& printer) override{
        printer << "(";
        printer << "sizeof(";
        type->dump(printer);
        if(declarator != nullptr){
            declarator->dump(printer);
        }
        printer << ")";
        printer << ")";
    }
};


