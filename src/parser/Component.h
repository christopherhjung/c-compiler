//
// Created by Christopher Jung on 19.11.20.
//

#pragma once

#include <iostream>
#include "../utils/Comparable.h"

class Component : public Comparable<Component>{
public:
    bool flag = true;
    virtual ~Component() = default;

    void initialize() {
        if(flag){
            flag = false;
            update();
        }
    }

    virtual void update(){};
};
