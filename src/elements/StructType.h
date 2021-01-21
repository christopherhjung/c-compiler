//
// Created by chris on 21.01.21.
//

#pragma once

#include <string>
#include "Elements.h"

class StructType : public Type{
public:
    const std::string *name;
    std::vector<Declaration*> declarations;

    void dump(PrettyPrinter& printer) override{
        printer << "struct";

        if(name != nullptr){
            printer << " " << *name;
        }

        if(!declarations.empty()){
            printer.newLine();
            printer << "{";
            printer.newLine();
            printer.increaseDepth();
            for(auto decl : declarations){
                decl->dump(printer);
                printer << ";";
                printer.newLine();
            }
            printer.decreaseDepth();
            printer << "}";
        }
    }
};


