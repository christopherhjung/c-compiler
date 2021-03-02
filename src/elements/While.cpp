//
// Created by chris on 21.01.21.
//

#include "While.h"
#include "../parser/PrettyPrinter.h"
#include "Util.h"
#include "Expression.h"
#include "../transform/TransformContext.h"

void While::dump(PrettyPrinter &printer) {
    printer << "while (";
    condition->dump(printer);
    printer << ")";
    printIndentIfNotBlock(printer, body);
}

void While::create(TransformContext &context){
    llvm::BasicBlock *conditionBlock = context.createBasicBlock("while-condition");
    llvm::BasicBlock *end;
    context.builder.CreateBr(conditionBlock);

    llvm::BasicBlock *startBody;
    if(body == nullptr){
        startBody = conditionBlock;
        end = context.createBasicBlock("while-end");
    }else{
        startBody = context.createBasicBlock("while-body");
        end = context.createBasicBlock("while-end");

        context.whileCondition = conditionBlock;
        context.whileEnd = end;

        context.setCurrentBlock(startBody);
        body->create(context);
        context.builder.CreateBr(conditionBlock);
    }

    context.setCurrentBlock(conditionBlock);
    condition->createConditionBranch(context, startBody, end);

    context.setCurrentBlock(end);
}