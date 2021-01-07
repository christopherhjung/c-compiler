//
// Created by Christopher Jung on 19.11.20.
//

#pragma once

#include <unordered_set>

#include "../utils/Comparable.h"
#include "Rule.h"

class Rule;
class Entity;

class Item : public Comparable<Item>{
public:
    uint32_t dot;
    Rule* rule;
    std::unordered_set<Entity*> lookahead;

    bool isFinish(){
        return dot >= rule->keys.size();
    }

    uint64_t hash() const override;

    bool equals(const Item *other) const override;
};
