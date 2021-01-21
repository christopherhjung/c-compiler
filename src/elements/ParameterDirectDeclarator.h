//
// Created by chris on 21.01.21.
//

#pragma once

#include "Elements.h"

class ParameterDirectDeclarator : public DirectDeclarator{
public:
    DirectDeclarator* directDeclarator = nullptr;
    ParameterTypeList* parameterTypeList = nullptr;

    void dump(PrettyPrinter& printer) override{
        printer << "(";
        if(directDeclarator != nullptr){
            directDeclarator->dump(printer);
        }

        if(parameterTypeList != nullptr){
            printer << "(";
            parameterTypeList->dump(printer);
            printer << ")";
        }
        printer << ")";
    }
};



