//
// Created by Christopher Jung on 19.11.20.
//

#pragma once

#include "Component.h"
#include "Rule.h"
#include "Entity.h"

class Grammar : public Component {

public:
    Entity* end;
    Rule* init;
    std::vector<Rule*> rules;
    std::vector<Entity*> terminals;
    std::vector<Entity*> nonTerminals;

    Grammar(Entity* end, Rule* init, std::vector<Entity*> entities, std::vector<Rule*> rules) : end(end), init(init), rules(rules){
        for(Entity*& entity : entities){
            if(entity->terminal){
                terminals.push_back(entity);
            }else{
                nonTerminals.push_back(entity);
            }
        }
    }

    void build() override {
        for(Rule* rule : rules){
            rule->initialize();
        }

        for(Entity*& entity : terminals){
            entity->initialize();
        }

        for(Entity*& entity : nonTerminals){
            entity->initialize();
        }
    }

    uint64_t hash() const override {
        return 0;
    }

    bool equals(const Comparable *other) const override {
        return other == this;
    }
};
