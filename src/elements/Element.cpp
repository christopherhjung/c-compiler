//
// Created by chris on 21.01.21.
//

#include "Element.h"

#include "../parser/PrettyPrinter.h"

std::string Element::toString() {
    std::stringstream ss;
    PrettyPrinter prettyPrinter(ss);
    dump(prettyPrinter);
    return ss.str();
}