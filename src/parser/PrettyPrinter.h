//
// Created by Christopher Jung on 04.12.20.
//

#pragma once

#include <iostream>

class PrettyPrinter {
    std::ostream &stream;
    int32_t depth = 0;
    bool needIndent = true;
public:
    PrettyPrinter(std::ostream &stream) : stream(stream) {

    }

    void print(const std::string &str) {
        stream << str;
    }

    PrettyPrinter &operator<<(const std::string &str) {
        if (needIndent) {
            needIndent = false;
            for (int32_t i = depth; i > 0; i--) {
                stream << "\t";
            }
        }
        stream << str;
        return *this;
    }

    void increaseDepth() {
        depth++;
    }

    void decreaseDepth() {
        depth--;
    }

    void newLine() {
        stream << "\n";
        needIndent = true;
    }

    void withoutIndent() {
        needIndent = false;
    }
};
