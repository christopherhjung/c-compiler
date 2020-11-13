//
// Created by Christopher Jung on 12.11.20.
//

#pragma once
#include <string>
#include <iostream>

class Location {
    std::string fileName;
    int line;
    int column;
public:
    Location(const std::string& fileName, int line, int column) : fileName(fileName), line(line), column(column){

    };

    friend std::ostream& operator<<(std::ostream& stream, const Location& tok){
        return stream << tok.fileName << ":" << tok.line << ":" << tok.column;
    }
};
