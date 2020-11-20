//
// Created by Christopher Jung on 19.11.20.
//

#pragma once

#include <vector>
#include <unordered_set>
#include "Item.h"
#include "../utils/Comparable.h"

class Closure {
public:
    uint32_t id;
    std::unordered_set<Item*> items;
    std::unordered_map<Entity*, Closure*> transitions;

    Closure(uint32_t id) : id(id){

    }
};
