//
// Created by Christopher Jung on 10.11.20.
//

#ifndef C4_TOKEN_H
#define C4_TOKEN_H

#include <string>
#include <iostream>
#include <utility>

struct Token {

public:
    int line;
    int column;
    int id;
    std::string name;
    std::string* value;
    Token(int line, int column, int id, std::string  name, std::string* value) : line(line), column(column), id(id), name(std::move(name)), value(value){

    };

    friend std::ostream& operator<<(std::ostream& stream, const Token& tok){
        return stream << tok.line << ":" << tok.column << ": " << tok.name << " " << *(tok.value) ;
    }
};



#endif //C4_TOKEN_H
