//
// Created by chris on 21.01.21.
//

#pragma once

#include <string>

class PrettyPrinter;
class Statement;

template<typename Base, typename T>
inline bool instanceof(const T *obj) {
    return dynamic_cast<const Base *>(obj) != nullptr;
}

void printIndentIfNotBlock(PrettyPrinter &printer, Statement *statement);

void printStatement(PrettyPrinter &printer, Statement *statement);

void replaceAll(std::string& str, const std::string& from, const std::string& to);

std::string escapeString(const std::string& str);