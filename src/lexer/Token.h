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
    Location* location;
    uint32_t id;
    const std::string& name;
    const std::string value;
    Token(Location* location, int id, const std::string&  name, const std::string& value) : location(location), id(id), name(name), value(value){

    };

    virtual ~Token() {
        delete(location);
    }

    friend std::ostream& operator<<(std::ostream& stream, const Token& tok){
        return stream << *tok.location << ": " << tok.name << " " << tok.value;
    }
};


