//
// Created by chris on 21.01.21.
//

#pragma once

#include "DirectDeclarator.h"

class PrettyPrinter;

class Declarator : public DirectDeclarator {
public:
    int pointer = 0;
    DirectDeclarator *directDeclarator = nullptr;

    void dump(PrettyPrinter &printer) override;
};


