//
// Created by chris on 21.01.21.
//

#pragma once

#include "Statement.h"

class PrettyPrinter;

class Continue : public Statement {
public:

    void dump(PrettyPrinter &printer) override;
};



