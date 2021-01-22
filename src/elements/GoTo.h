//
// Created by chris on 21.01.21.
//

#pragma once

#include <string>
#include "Statement.h"

class PrettyPrinter;

class GoTo : public Statement {
public:
    const std::string *name;

    void dump(PrettyPrinter &printer) override;

    void create(TransformContext &context) override;
};



