//
// Created by Christopher Jung on 12.11.20.
//

#pragma once

#include <string>
#include <unordered_set>

struct SharedPtrHash {
    std::hash<std::string> hash = std::hash<std::string>();

    size_t operator()(const std::shared_ptr<std::string> & s) const {
        return hash(*s);
    }
};

struct SharedPtrEqualTo {

    bool operator() ( const std::shared_ptr<std::string>& lhs, const std::shared_ptr<std::string>& rhs) const
    {
        return *lhs == *rhs;
    }
};

class SymbolCache {
    std::unordered_set<std::string> cache;
public:
    explicit SymbolCache(size_t preAllocSize = 512) : cache(preAllocSize) {}

    const std::string internalize(const std::string& str) {
        return (*cache.emplace(str).first);
    }
};
