//
// Created by chris on 21.01.21.
//

#pragma once

#include "DirectDeclarator.h"

class PrettyPrinter;

class ParameterTypeList;

class ParameterDirectDeclarator : public DirectDeclarator {
public:
    DirectDeclarator *directDeclarator = nullptr;
    ParameterTypeList *parameterTypeList = nullptr;

    void dump(PrettyPrinter &printer) override;
};



