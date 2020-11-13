//
// Created by Christopher Jung on 10.11.20.
//

#pragma once

#include <string>
#include <iostream>
#include <utility>
#include "Location.h"

static std::string ss = "";

struct Token {

public:
    Location* location = nullptr;
    uint32_t id;
    std::string& name;
    std::string value;
    Token( ) : name(ss) {
    };
    Token(Location* location, int id, std::string&  name, std::string& value) : location(location), id(id), name(name), value(value){

    };

    virtual ~Token() {
        delete(location);
        location = nullptr;
    }

    friend std::ostream& operator<<(std::ostream& stream, const Token& tok){
        return stream << *tok.location << ": " << tok.name << " " << tok.value;
    }
};


