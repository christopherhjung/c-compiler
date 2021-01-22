//
// Created by chris on 21.01.21.
//

#pragma once


#include "Statement.h"

class PrettyPrinter;

class Break : public Statement {
public:
    void dump(PrettyPrinter &printer) override;

    void create(TransformContext &context) override;
};



