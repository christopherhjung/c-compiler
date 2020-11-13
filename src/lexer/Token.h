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
    std::unique_ptr<Location> location;
    uint32_t id;
    std::string name;
    std::string* value;
    Token(std::unique_ptr<Location>& location, int id, std::string  name, std::string* value) : location(std::move(location)), id(id), name(std::move(name)), value(value){

    };

    friend std::ostream& operator<<(std::ostream& stream, const Token& tok){
        return stream << *tok.location << ": " << tok.name << " " << *tok.value;
    }
};


