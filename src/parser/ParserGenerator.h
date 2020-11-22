//
// Created by Christopher Jung on 19.11.20.
//

#pragma once

#include <string>
#include <functional>
#include <unordered_map>
#include <iomanip>
#include <vector>
#include <unordered_set>
#include "Item.h"
#include "Grammar.h"
#include "ParserTable.h"

class Closure {
public:
    uint32_t id;
    std::unordered_set<Item*, Comparable::Hash, Comparable::Equals> items;
    std::unordered_map<Entity*, Closure*> transitions;

    Closure(uint32_t id) : id(id){

    }
};

class ParserGenerator {
    uint32_t currentClosure = 0;
    std::unordered_map<Item*, Closure*, Comparable::Hash, Comparable::Equals> closures;

    void computeClosure(Closure* closure){
        for(Item* sub : closure->items){
            if(sub->dot >= sub->rule->keys.size()){
                continue;
            }

            Item* next = new Item();
            next->rule = sub->rule;
            next->lookahead = sub->lookahead;
            next->dot = sub->dot + 1;


            Closure* nextClosure = createClosure(next);
            Entity* consumedEntity = sub->rule->keys[sub->dot];
            closure->transitions[consumedEntity] = nextClosure;
        }
    }


    void createClosure(Item* base, Closure* closure){
        if(closure->items.find(base) != closure->items.end()){
            return;
        }

        closure->items.insert(base);

        Rule* rule = base->rule;
        if(rule->keys.size() > base->dot){
            Entity* next = rule->keys[base->dot];
            std::unordered_set<Entity*>* lookahead;
            if(rule->keys.size() > base->dot + 1){
                Entity* test = rule->keys[base->dot + 1];
                lookahead = &test->lookahead;
            }else{
                lookahead = &base->lookahead;
            }

            for(Rule* nextRule : next->productions){
                Item* nextItem = new Item();
                nextItem->dot = 0;
                nextItem->rule = nextRule;
                nextItem->lookahead = *lookahead;
                createClosure(nextItem, closure);
            }
        }
    }

    Closure* createClosure(Item* base){
        if(closures.find(base) != closures.end()){
            return closures[base];
        }

        auto* closure = new Closure(currentClosure++);
        closures[base] = closure;
        createClosure(base, closure);
        computeClosure(closure);
        return closure;
    }



    void init(Grammar* grammar){
        grammar->initialize();
        Rule* init = grammar->init;
        Item* item = new Item();
        item->dot = 0;
        item->rule = init;
        item->lookahead.insert(grammar->end);
        createClosure(item);
    }

public:
    ParserTable* generateParser(Grammar* grammar){
        init(grammar);

        auto* table = new ParserTable(closures.size());
        for(const auto& pair : closures){
            Closure* closure = pair.second;
            auto* entry = new Entry(grammar);
            table->entries[closure->id] = entry;

            for( const auto& trigger : closure->transitions ){
                Entity* entity = trigger.first;
                if(entity->terminal){
                    entry->actions[entity->id] = trigger.second->id;
                }else{
                    entry->jumps[entity->id] = trigger.second->id;
                }
            }

            for( Item* item : closure->items){
                if(item->isFinish()){
                    for( Entity* lookahead : item->lookahead ){
                        entry->restore[lookahead->id] = item->rule;
                    }
                }
            }
        }

        return table;
    }
};
