//
// Created by chris on 21.01.21.
//

#pragma once

#include <string>
#include "Elements.h"

class GoTo : public Statement{
public:
    const std::string *name;

    void dump(PrettyPrinter& printer) override{
        printer << "goto " << *name;
    }
};



