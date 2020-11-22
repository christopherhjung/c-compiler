//
// Created by Christopher Jung on 19.11.20.
//

#pragma once

#include <vector>
#include <string>
#include "Component.h"
class Rule;

class Entity : public Component{
public:
    uint32_t id;
    std::string name;
    bool terminal;
    std::vector<Rule*> productions;
    std::unordered_set<Entity*> lookahead;
    Entity(uint32_t id ,const std::string& name, bool terminal) : id(id), name(name), terminal(terminal){

    }

    Entity(uint32_t id, const std::string& name) : id(id), name(name), terminal(false){

    }

    void update() override;

    uint64_t hash() const override;

    bool equals(const Comparable *other) const override;
};
