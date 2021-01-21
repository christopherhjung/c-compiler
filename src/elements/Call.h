//
// Created by chris on 21.01.21.
//

#pragma once


#include "Expression.h"
#include "../lexer/Location.h"

class PrettyPrinter;

class Call : public Expression {
public:
    Expression *target = nullptr;
    std::vector<Expression *> values;
    std::vector<Location> locations;

    void dump(PrettyPrinter &printer) override;
};



