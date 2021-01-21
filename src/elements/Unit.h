//
// Created by chris on 21.01.21.
//

#pragma once

#include <vector>
#include "Elements.h"
#include "../transform/TransformContext.h"

class Unit : public Element{
public:
    std::vector<Element*> children;

    void dump(PrettyPrinter& printer) override{
        bool first = true;
        for(auto child : children){
            if(first){
                first = false;
            }else{
                printer.newLine();
                printer.newLine();
            }
            child->dump(printer);
            if(instanceof<Declaration>(child)){
                printer << ";";
            }
        }
    }

    void create(TransformContext& context){
        for( auto element : children ){
            if( auto method = dynamic_cast<Method*>(element) ){
                method->create(context);
            }else if( auto decl = dynamic_cast<Declaration*>(element) ){

            }
        }
    }
};



