

//
// Created by Christopher Jung on 11.11.20.
//

#pragma once

#include "Location.h"
#include <string>
#include <iostream>

class Error {
public:
    Location* location;
    std::string msg;
    Error(Location* location, const std::string& msg) : location(location),  msg(msg){

    };

    virtual ~Error() {

    }

    friend std::ostream& operator<<(std::ostream& stream, const Error& tok){
        return stream << *(tok.location) << ": error: " << tok.msg;
    }
};
