//
// Created by chris on 21.01.21.
//

#include "StringLiteral.h"
#include "../parser/PrettyPrinter.h"
#include "../transform/TransformContext.h"

void replaceAll(std::string& str, const std::string& from, const std::string& to) {
    if(from.empty())
        return;
    size_t start_pos = 0;
    while((start_pos = str.find(from, start_pos)) != std::string::npos) {
        str.replace(start_pos, from.length(), to);
        start_pos += to.length(); // In case 'to' contains 'from', like replacing 'x' with 'yx'
    }
}

void StringLiteral::dump(PrettyPrinter &printer) {
    printer << *value;
}

llvm::Value *StringLiteral::createRightValue(TransformContext &context){

    //auto test = context.resetAllocBuilder().CreateAlloca(context.builder.getInt8PtrTy());
    std::string str = *value;
    str.pop_back();
    str.erase(str.begin());

    replaceAll(str,"\\0", "\0");
    replaceAll(str,"\\a", "\a");
    replaceAll(str,"\\b", "\b");
    replaceAll(str,"\\f", "\f");
    replaceAll(str,"\\n", "\n");
    replaceAll(str,"\\r", "\r");
    replaceAll(str,"\\t", "\t");
    replaceAll(str,"\\v", "\v");
    replaceAll(str,"\\\"", "\"");

    llvm::Value* globalPtr = context.builder.CreateGlobalStringPtr(str);
    //context.builder.CreateStore( globalPtr, test);
    return globalPtr;
}