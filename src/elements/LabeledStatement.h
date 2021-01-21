//
// Created by chris on 21.01.21.
//

#pragma once

#include "Statement.h"

class PrettyPrinter;

class LabeledStatement : public Statement {
public:
    Statement *statement = nullptr;
    const std::string *name = nullptr;

    void dump(PrettyPrinter &printer) override;
};



