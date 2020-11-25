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
#include "../utils/PrintUtils.h"

class Closure {
public:
    uint32_t id;
    std::unordered_set<Item*, Comparable::Hash, Comparable::Equals> items;
    std::unordered_map<Entity*, Closure*> transitions;

    Closure(uint32_t id) : id(id){

    }
};

class Kernel : public Comparable {
public:
    std::unordered_set<Item*, Comparable::Hash, Comparable::Equals> seeds;

    uint64_t hash() const override {
        uint64_t result{31};
        for(auto* item : seeds){
            result ^= item->hash();
        }
        return result;
    }

    bool equals(const Comparable *other) const override {
        if(auto* kernel = dynamic_cast<const Kernel*>(other)) {
            for(auto kernelItem : kernel->seeds){
                if(seeds.find(kernelItem) == seeds.end()){
                    return false;
                }
            }
            //bool result = this->seeds == kernel->seeds;
            return true;
        }
        return false;
    }
};


void printClosure(const Closure* closure){
    std::cout << closure->id << std::endl;
    for(Item* item: closure->items){
        printItem(item);
    }
    std::cout << "next:" << std::endl;
    for(const auto& pair: closure->transitions){
        std::cout << pair.first->id << ": " << pair.first->name << ": " << pair.second->id << std::endl;

    }
    std::cout << "---------------------------------------" << std::endl;
}



void printKernel(const Kernel* closure){
    for(Item* item: closure->seeds){
        printItem(item);
    }
    std::cout << "---------------------------------------" << std::endl;
}


class ParserGenerator {
    uint32_t currentClosure = 0;
    std::unordered_map<Kernel*, Closure*, Comparable::Hash, Comparable::Equals> closures;

    void computeClosure(Closure* closure){
        std::unordered_map<Entity*, Kernel*> aggregate;
        std::unordered_set<Entity*> entities;

        printClosure(closure);
        for(Item* sub : closure->items){
            if(sub->dot >= sub->rule->keys.size()){

                Entity* final = sub->rule->keys[sub->dot - 1];
                if(entities.find(final) != entities.end()){
                    throw std::exception();
                }

                entities.insert(final);
                continue;
            }

            Item* next = new Item();
            next->rule = sub->rule;
            next->lookahead = sub->lookahead;
            next->dot = sub->dot + 1;
            Entity* consumedEntity = sub->rule->keys[sub->dot];

            Kernel* kernel;
            if(aggregate.find(consumedEntity) != aggregate.end()){
                kernel = aggregate[consumedEntity];
            }else{
                aggregate[consumedEntity] = kernel = new Kernel();
            }

            kernel->seeds.insert(next);
        }

        for(auto& pair : aggregate){
            auto key = pair.first;
            if(entities.find(key) != entities.end()){
                throw std::exception();
            }

            printKernel(pair.second);
            std::cout << pair.second->hash() << std::endl;

            std::cout << pair.second->hash() << std::endl;

            Closure* nextClosure = createClosure(pair.second);
            closure->transitions[pair.first] = nextClosure;
        }
    }

    void createClosure(Kernel* base, Closure* closure){
        for(auto* item : base->seeds){
            createClosure(item, closure);
        }
    }

    void createClosure(Item* base, Closure* closure){
        if(closure->items.find(base) != closure->items.end()){
            return;
        }

        closure->items.insert(base);

        //printItem(base);

        Rule* rule = base->rule;
        if(rule->keys.size() > base->dot){
            Entity* next = rule->keys[base->dot];
            std::unordered_set<Entity*>* lookahead = nullptr;
            if(rule->keys.size() > base->dot + 1){
                uint32_t i = base->dot;
                for( ; ++i < rule->keys.size(); ){
                    Entity* test = rule->keys[i];
                    lookahead = &test->lookahead;
                    if(!lookahead->empty()){
                        break;
                    }
                }

                if(lookahead == nullptr || lookahead->empty()){
                    lookahead = &base->lookahead;
                }
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

    Closure* createClosure(Kernel* base){


        if(closures.find(base) != closures.end()){
            return closures[base];
        }

        auto* closure = new Closure(currentClosure++);
        closures[base] = closure;
        createClosure(base, closure);
        minifyClosure(closure);
        printClosure(closure);
        computeClosure(closure);
        return closure;
    }

    void minifyClosure(Closure* closure){
        std::unordered_map<Rule*, std::unordered_map<uint32_t, std::unordered_set<Entity*>>> cache;

        for( Item* item : closure->items ){
            auto& set = cache[item->rule][item->dot];
            set.insert(item->lookahead.begin(), item->lookahead.end());
            //delete(item);
        }

        closure->items.clear();

        for( auto& ruleCache : cache ){
            for( auto& dotCache : ruleCache.second ){
                auto* item = new Item();
                item->dot = dotCache.first;
                item->rule = ruleCache.first;
                item->lookahead = dotCache.second;
                closure->items.insert(item);
            }
        }
    }

    void init(Grammar* grammar){
        grammar->initialize();
        Rule* init = grammar->root;
        Item* item = new Item();
        item->dot = 0;
        item->rule = init;
        item->lookahead.insert(grammar->end);

        auto* kernel = new Kernel();
        kernel->seeds.insert(item);

        createClosure(kernel);
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
                    if(entry->actions[entity->id] != -1){
                        throw std::exception();
                    }

                    entry->actions[entity->id] = trigger.second->id;
                }else{
                    if(entry->jumps[entity->id] != -1){
                        throw std::exception();
                    }

                    entry->jumps[entity->id] = trigger.second->id;
                }




            }

            for( Item* item : closure->items){
                if(item->isFinish()){
                    for( Entity* lookahead : item->lookahead ){
                        if(entry->restore[lookahead->id] != nullptr){
                            throw std::exception();
                        }
                        entry->restore[lookahead->id] = item->rule;
                    }
                }
            }
        }

        return table;
    }
};
