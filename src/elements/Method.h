//
// Created by chris on 21.01.21.
//

#pragma once

#include "Elements.h"

class Method : public Element{
public:
    Declaration* declaration = nullptr;
    Block* body = nullptr;

    void dump(PrettyPrinter& printer) override{
        declaration->dump(printer);
        printer.newLine();
        body->dump(printer);
    }

    void create(TransformContext& context){
        auto entry = context.createFunction("main");
        body->create(context, entry);
    }
};



