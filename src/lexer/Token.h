//
// Created by Christopher Jung on 10.11.20.
//

#pragma once

#include <string>
#include <iostream>
#include <utility>
#include "Location.h"

struct Token {

public:
    Location location;
    int id;
    std::string name;
    std::string* value;
    Token(Location location, int id, std::string  name, std::string* value) : location(std::move(location)), id(id), name(std::move(name)), value(value){

    };

    friend std::ostream& operator<<(std::ostream& stream, const Token& tok){
        return stream << tok.location << ": " << tok.name << " " << *(tok.value) ;
    }
};


