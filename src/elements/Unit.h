//
// Created by chris on 21.01.21.
//

#pragma once

#include <vector>
#include "Element.h"


class PrettyPrinter;
class Scope;
class TransformContext;

class Unit : public Element {
public:
    std::vector<Element *> children;
    Scope* scope;

    void dump(PrettyPrinter &printer) override;

    void create(TransformContext &context);
};



