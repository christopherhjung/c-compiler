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
    Rule* root;
    std::vector<Rule*> rules;
    std::vector<Entity*> terminals;
    std::vector<Entity*> nonTerminals;

    Grammar(Entity* end, Rule* init, std::vector<Entity*> entities, const std::vector<Rule*>& rules) : end(end), root(init), rules(rules){
        for(Entity*& entity : entities){
            if(entity->terminal){
                terminals.push_back(entity);
            }else{
                nonTerminals.push_back(entity);
            }
        }

        //std::sort(nonTerminals.begin(), nonTerminals.end(), [](Entity* a,Entity* b){return a->id < b->id ? 1 : a->id == b->id ? 0 : -1; });
    }

    void update() override {
        for(Rule* rule : rules){
            rule->initialize();
        }

        for(Entity*& entity : terminals){
            entity->initialize();
        }

        for(uint32_t i = 0 ; i < rules.size(); i++){
            for(Entity*& entity : nonTerminals){
                entity->update();
            }
        }
    }

    uint64_t hash() const override {
        return 0;
    }

    bool equals(const Comparable *other) const override {
        return other == this;
    }
};
