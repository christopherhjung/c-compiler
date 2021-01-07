//
// Created by Christopher Jung on 19.11.20.
//

#pragma once


#include <cstdint>

template<class T>
class Comparable {
public:
    struct Hash {
        uint64_t operator()(const T* s) const {
            return s->hash();
        }
    };

    struct Equals {
        uint64_t operator()(const T* l, const T* r) const {
            return l->equals(r);
        }
    };

    virtual uint64_t hash() const{
        return 0;
    };
    virtual bool equals(const T* other) const = 0;

    virtual bool operator==(const T& other) const {
        return this->equals(&other);
    }
};
