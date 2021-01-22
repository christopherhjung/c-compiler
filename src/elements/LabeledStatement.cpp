//
// Created by chris on 21.01.21.
//

#include "LabeledStatement.h"
#include "../parser/PrettyPrinter.h"
#include "Util.h"
#include "../transform/TransformContext.h"

void LabeledStatement::dump(PrettyPrinter &printer) {
    printer.withoutIndent();
    printer << *name << ":";
    printer.newLine();
    printStatement(printer, statement);
}

void LabeledStatement::create(TransformContext &context){
    auto bb = context.getJumpTarget(name);
    context.builder.CreateBr(bb);
    context.setCurrentBlock(bb);
    if(statement != nullptr){
        statement->create(context);
    }
}