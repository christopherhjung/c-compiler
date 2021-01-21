//
// Created by chris on 21.01.21.
//

#pragma once


#include "Elements.h"
class Break : public Statement{
public:
    void dump(PrettyPrinter& printer) override{
        printer << "break";
    }
};



