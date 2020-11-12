

//
// Created by Christopher Jung on 11.11.20.
//

#pragma once

#include "Location.h"
#include <string>
#include <iostream>

class Error {
public:
    std::unique_ptr<Location> location;
    std::string msg;
    Error(std::unique_ptr<Location>& location, std::string msg) : location(std::move(location)),  msg(std::move(msg)){

    };

    friend std::ostream& operator<<(std::ostream& stream, const Error& tok){
        return stream << *(tok.location) << ": error: " << tok.msg;
    }
};
