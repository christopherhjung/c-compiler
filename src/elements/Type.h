//
// Created by chris on 21.01.21.
//

#pragma once

#include <functional>
#include "Element.h"

class PrettyPrinter;

class Type : public Element {
public:
    uint32_t type;

    void dump(PrettyPrinter &printer) override;
};


