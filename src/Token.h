//
// Created by Christopher Jung on 10.11.20.
//

#ifndef C4_TOKEN_H
#define C4_TOKEN_H

#include <string>
#include <iostream>

struct Token {

public:
    int line;
    int column;
    int id;
    std::string* value;
    Token(int line, int column, int id, std::string* value) : line(line), column(column), id(id), value(value){

    };

    friend std::ostream& operator<<(std::ostream& stream, const Token& tok){
        stream << tok.line << ":" << tok.column << ":" << tok.id << ":" << *(tok.value) ;
        return stream;
    }
};



#endif //C4_TOKEN_H
