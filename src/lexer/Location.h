//
// Created by Christopher Jung on 12.11.20.
//

#pragma once

#include <string>
#include <iostream>

class Location {
public:
    std::string fileName = "";
    int32_t line = 0;
    int32_t column = 0;

    friend std::ostream &operator<<(std::ostream &stream, const Location &tok) {
        return stream << tok.fileName << ":" << tok.line << ":" << tok.column;
    }
};
