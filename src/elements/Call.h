//
// Created by chris on 21.01.21.
//

#pragma once


#include "Elements.h"
class Call : public Expression{
public:
    Expression* target = nullptr;
    std::vector<Expression*> values;
    std::vector<Location> locations;

    void dump(PrettyPrinter& printer) override{
        printer << "(";
        target->dump(printer);
        printer << "(";
        bool first = true;
        for(auto val : values){
            if(first){
                first = false;
            }else{
                printer << ", ";
            }
            val->dump(printer);
        }
        printer << ")";
        printer << ")";
    }
};



