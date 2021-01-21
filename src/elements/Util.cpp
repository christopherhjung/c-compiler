//
// Created by chris on 21.01.21.
//

#include "Util.h"

#include "Block.h"
#include "If.h"
#include "While.h"
#include "LabeledStatement.h"

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
