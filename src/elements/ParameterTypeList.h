//
// Created by chris on 21.01.21.
//

#pragma once

#include <vector>
#include "../lexer/Location.h"
#include "Dumpable.h"

class PrettyPrinter;

class Declaration;

class ParameterTypeList : public Dumpable {
public:
    std::vector<Location> locations;
    std::vector<Declaration *> declarations;

    void dump(PrettyPrinter &printer) override;
};


