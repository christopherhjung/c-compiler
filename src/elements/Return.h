//
// Created by chris on 21.01.21.
//

#pragma once

#include "Elements.h"

class Return : public Statement{
public:
    Expression* value = nullptr;

    void dump(PrettyPrinter& printer) override{
        printer << "return";
        if(value != nullptr){
            printer << " ";
            value->dump(printer);
        }
    }
};


