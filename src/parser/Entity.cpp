//
// Created by Christopher Jung on 19.11.20.
//
#include <unordered_set>

#include "Entity.h"
#include "Rule.h"

void Entity::build() {

    for(Rule* rule : this->productions){
        Entity* start = rule->keys[0];

        if(start->terminal){
            this->lookahead.insert(start);
        }else{
            start->initialize();
            this->lookahead.insert(start->lookahead.begin(), start->lookahead.end());
        }
    }
}

uint64_t Entity::hash() const {
    return (uint64_t)this;
}

bool Entity::equals(const Comparable *other) const {
    return other == this;
}
