//
// Created by Christopher Jung on 04.12.20.
//

#include "SimpleObjects.h"
#include "PrettyPrinter.h"
#include <iostream>

void parser::Declarator::dump(PrettyPrinter &stream) {
    for(uint32_t i = 0 ; i < pointer ; i++){
        stream << "*";
    }
    if(directDeclarator != nullptr){
        directDeclarator->dump(stream);
    }
}

void parser::printIndentIfNotBlock(PrettyPrinter& stream, Statement* statement){
    {
        if(instanceof<Block>(statement)){
            stream << " ";
            statement->dump(stream);
        }else{
            stream.newLine();
            stream.increaseDepth();
            statement->dump(stream);
            if(!instanceof<If>(statement) && !instanceof<While>(statement) && !instanceof<Block>(statement)){
                stream << ";";
            }
            stream.decreaseDepth();
        }
    }
}