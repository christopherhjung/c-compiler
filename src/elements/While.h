//
// Created by chris on 21.01.21.
//

#pragma once

#include "Statement.h"


class PrettyPrinter;

class Expression;

class While : public Statement {
public:
    Expression *condition = nullptr;
    Statement *body = nullptr;

    void dump(PrettyPrinter &printer) override;
};


