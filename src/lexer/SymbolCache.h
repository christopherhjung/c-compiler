//
// Created by Christopher Jung on 12.11.20.
//

#pragma once

#include <string>
#include <unordered_set>
#include <functional>

class SymbolCache {
    std::unordered_set<std::string> cache;
public:
    explicit SymbolCache(size_t preAllocSize = 512) : cache(preAllocSize) {}

    const std::string &internalize(const std::string &str) {
        return (*cache.emplace(str).first);
    }
};
