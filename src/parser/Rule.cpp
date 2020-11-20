//
// Created by Christopher Jung on 19.11.20.
//

#include "Rule.h"
#include "Entity.h"

void Rule::build() {
    this->owner->productions.push_back(this);
}

uint64_t Rule::hash() const {
    return (uint64_t)this;
}

bool Rule::equals(const Comparable *other) const {
    return other == this;
}
