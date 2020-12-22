//
// Created by Christopher Jung on 04.12.20.
//

#include "SimpleObjects.h"
#include "PrettyPrinter.h"
#include <iostream>

void Declarator::dump(PrettyPrinter &stream) {

    for(uint32_t i = 0 ; i < pointer ; i++){
        stream << "(*";
    }
    if(directDeclarator != nullptr){
        directDeclarator->dump(stream);
    }
    for(uint32_t i = 0 ; i < pointer ; i++){
        stream << ")";
    }
}

void printIndentIfNotBlock(PrettyPrinter& stream, Statement* statement){
    if(instanceof<Block>(statement)){
        stream << " ";
        statement->dump(stream);
    }else{
        stream.newLine();
        stream.increaseDepth();
        printStatement(stream, statement);
        stream.decreaseDepth();
    }
}

void printStatement(PrettyPrinter &stream, Statement *statement) {
    if(statement != nullptr){
        statement->dump(stream);
    }

    if(!instanceof<If>(statement) && !instanceof<While>(statement) &&
            !instanceof<Block>(statement) && !instanceof<LabeledStatement>(statement)){
        stream << ";";
    }
}

