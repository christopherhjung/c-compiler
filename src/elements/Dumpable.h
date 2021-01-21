//
// Created by chris on 21.01.21.
//

#pragma once

#include "../parser/PrettyPrinter.h"

class Dumpable{
public:
    virtual void dump(PrettyPrinter& printer) = 0;
};


