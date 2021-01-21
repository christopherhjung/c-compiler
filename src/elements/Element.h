//
// Created by chris on 21.01.21.
//

#pragma once

#include <string>
#include <sstream>

#include "../lexer/Location.h"
#include "../parser/PrettyPrinter.h"
#include "Elements.h"

class Element : public Dumpable{
public:
    Location location;
    virtual ~Element() = default;

    std::string toString(){
        std::stringstream ss;
        PrettyPrinter prettyPrinter(ss);
        dump(prettyPrinter);
        return ss.str();
    }
};


