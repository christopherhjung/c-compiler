//
// Created by chris on 21.01.21.
//

#pragma once

#include "../lexer/Location.h"
#include "Elements.h"

class ParameterTypeList : public Dumpable {
public:
    std::vector<Location> locations;
    std::vector<Declaration *> declarations;

    void dump(PrettyPrinter& printer) override{
        bool first = true;
        for(auto decl : declarations){
            if(first){
                first = false;
            }else{
                printer << ", ";
            }

            decl->dump(printer);
        }
    }
};


