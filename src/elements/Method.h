//
// Created by chris on 21.01.21.
//

#pragma once

#include "Element.h"

class PrettyPrinter;

class Declaration;

class Block;

class TransformContext;

class Method : public Element {
public:
    Declaration *declaration = nullptr;
    Block *body = nullptr;

    void dump(PrettyPrinter &printer) override;

    void create(TransformContext &context);
};



