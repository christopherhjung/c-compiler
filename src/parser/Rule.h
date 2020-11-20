//
// Created by Christopher Jung on 19.11.20.
//

#pragma once

#include <string>
#include <unordered_set>
#include <vector>


#include "Component.h"

class Entity;

class Rule : public Component{
public:
    uint32_t id;
    Entity* owner;
    std::vector<Entity*> keys;

    Rule(uint32_t id, Entity* owner, std::vector<Entity*> keys): id(id), owner(owner), keys(keys){

    }

    void build() override;

    uint64_t hash() const override;

    bool equals(const  Comparable *other) const override;
};
