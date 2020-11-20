//
// Created by Christopher Jung on 19.11.20.
//

#pragma once

#include "../utils/Comparable.h"

class Component : public Comparable{
public:
    bool flag = true;
    virtual ~Component() = default;

    void initialize() {
        if(flag){
            flag = false;
            build();
        }
    }

    virtual void build(){};
};
