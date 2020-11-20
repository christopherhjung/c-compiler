//
// Created by Christopher Jung on 19.11.20.
//

#pragma once


class Grammar : public Component {

public:
    Entity* end;
    Rule* init;
    std::vector<Rule*> rules;
    std::vector<Entity*> entities;
    Grammar(Entity* end, Rule* init, std::vector<Entity*> entities, std::vector<Rule*> rules) : end(end), init(init), entities(entities), rules(rules){

    }

    void build() override {
        for(Rule* rule : rules){
            rule->initialize();
        }

        for(Entity* entity : entities){
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
