//
// Created by Christopher Jung on 19.11.20.
//
#include <unordered_set>

#include "Entity.h"
#include "Rule.h"

void Entity::update() {
    if(terminal){
        this->lookahead.insert(this);
    }else{
        for(Rule* rule : this->productions){
            if(!rule->keys.empty()){
                Entity* start = rule->keys[0];
                this->lookahead.insert(start->lookahead.begin(), start->lookahead.end());
            }
        }
    }
}

uint64_t Entity::hash() const {
    return (uint64_t)this;
}

bool Entity::equals(const Comparable *other) const {
    return other == this;
}
