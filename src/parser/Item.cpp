//
// Created by Christopher Jung on 19.11.20.
//

#include "Item.h"
#include "Rule.h"
#include "Entity.h"

uint64_t Item::hash() const {
    uint64_t hash{31};
    for(auto* entity : lookahead){
        hash ^= entity->hash();
    }

    hash = hash * this->dot + 31;
    hash = hash * this->rule->hash();

    return hash;
}

bool Item::equals(const Comparable *other) const {
    if(const Item* item = dynamic_cast<const Item*>(other)) {
        return this->dot == item->dot && this->lookahead == item->lookahead && rule->equals(item->rule);
    }
    return false;
}
