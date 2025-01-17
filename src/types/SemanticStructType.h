//
// Created by chris on 21.01.21.
//

#pragma once

#include <unordered_map>
#include <vector>
#include "SemanticType.h"

class SemanticStructType : public SemanticType {
public:
    std::unordered_map<const std::string *, int> map;
    std::vector<const SemanticType *> types;
    bool defined = false;
    int size = -1;

    explicit SemanticStructType() {

    }

    explicit SemanticStructType(bool assignable) : SemanticType(assignable) {

    }

    bool equals(const SemanticType *other) const override {
        if (auto structType = other->asSemanticStructType()) {
            return structType == this;
        }
        return false;
    }

    const SemanticType *member(const IdentifierUse *identifier) const {
        if (identifier) {
            auto pos = map.find(identifier->value);

            if (pos == map.end()) {
                return nullptr;
            }

            return types[pos->second];
        }

        return nullptr;
    }

    const SemanticStructType *asSemanticStructType() const override {
        return this;
    }
};



