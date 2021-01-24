//
// Created by chris on 21.01.21.
//

#include "Util.h"

#include "Block.h"
#include "If.h"
#include "While.h"
#include "LabeledStatement.h"
#include "../parser/PrettyPrinter.h"

void printIndentIfNotBlock(PrettyPrinter &stream, Statement *statement) {
    if (instanceof<Block>(statement)) {
        stream << " ";
        statement->dump(stream);
    } else {
        stream.newLine();
        stream.increaseDepth();
        printStatement(stream, statement);
        stream.decreaseDepth();
    }
}

void printStatement(PrettyPrinter &stream, Statement *statement) {
    if (statement != nullptr) {
        statement->dump(stream);
    }

    if (!instanceof<If>(statement) && !instanceof<While>(statement) &&
        !instanceof<Block>(statement) && !instanceof<LabeledStatement>(statement)) {
        stream << ";";
    }
}

void replaceAll(std::string& str, const std::string& from, const std::string& to) {
    if(from.empty())
        return;
    size_t start_pos = 0;
    while((start_pos = str.find(from, start_pos)) != std::string::npos) {
        str.replace(start_pos, from.length(), to);
        start_pos += to.length(); // In case 'to' contains 'from', like replacing 'x' with 'yx'
    }
}

std::string escapeString(const std::string& str){
    std::string result = str;
    result.pop_back();
    result.erase(result.begin());

    replaceAll(result,"\\0", "\0");
    replaceAll(result,"\\a", "\a");
    replaceAll(result,"\\b", "\b");
    replaceAll(result,"\\f", "\f");
    replaceAll(result,"\\n", "\n");
    replaceAll(result,"\\r", "\r");
    replaceAll(result,"\\t", "\t");
    replaceAll(result,"\\v", "\v");
    replaceAll(result,"\\\"", "\"");

    return result;
}