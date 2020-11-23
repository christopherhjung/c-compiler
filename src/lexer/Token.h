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
    int32_t id;
    std::string value;
    Token( ){
    };

    friend std::ostream& operator<<(std::ostream& stream, const Token& tok){
        return stream << tok.location << " " << tok.value;
    }
};


