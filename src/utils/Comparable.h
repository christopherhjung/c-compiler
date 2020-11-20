//
// Created by Christopher Jung on 19.11.20.
//

#pragma once


#include <cstdint>

class Comparable {
public:
    struct Hash {
        uint64_t operator()(const Comparable* s) const {
            return s->hash();
        }
    };

    struct Equals {
        uint64_t operator()(const Comparable* l, const Comparable* r) const {
            return l->equals(r);
        }
    };

    virtual uint64_t hash() const = 0;
    virtual bool equals(const Comparable* other) const = 0;
};
