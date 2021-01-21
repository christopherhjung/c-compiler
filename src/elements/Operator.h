//
// Created by chris on 21.01.21.
//

#pragma once

#include <functional>
#include "Elements.h"

class Operator : public Element{
public:
    uint32_t id;

    void dump(PrettyPrinter& printer) override;
};



