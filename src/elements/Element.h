//
// Created by chris on 21.01.21.
//

#pragma once

#include <string>
#include <sstream>
#include "Dumpable.h"
#include "../lexer/Location.h"

class PrettyPrinter;

class Element : public Dumpable {
public:
    Location location;

    virtual ~Element() = default;

    std::string toString();
};


