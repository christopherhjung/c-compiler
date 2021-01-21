//
// Created by chris on 21.01.21.
//

#pragma once

#include <functional>
#include "Elements.h"
#include "../types/TypeDefines.h"

class Type : public Element{
public:
    uint32_t  type;

    void dump(PrettyPrinter& printer) override{
        if(type == TYPE_INT){
            printer << "int";
        }else if(type == TYPE_CHAR){
            printer << "char";
        }else if(type == TYPE_VOID){
            printer << "void";
        }
    }
};


