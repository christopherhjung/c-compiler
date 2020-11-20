//
// Created by Christopher Jung on 19.11.20.
//

#pragma once

#include <string>
#include <functional>
#include <unordered_map>
#include "Grammar.h"
#include "Closure.h"
#include "ParserTable.h"


class ParserGenerator {
    uint32_t currentClosure = 0;
    std::unordered_map<Item*, Closure*, Comparable::Hash, Comparable::Equals> closures;

    void printEntity(Entity* item){
        std::cout << item->name;
    }

    void printRule(Rule* rule){
        printEntity(rule->owner);

        std::cout << "->";
        uint32_t i = 0;
        for(Entity* entity : rule->keys){
            std::cout << " ";
            printEntity(entity);
            i++;
        }
    }

    void printItem(Item* item){
        printEntity(item->rule->owner);

        std::cout << "->";
        uint32_t i = 0;
        for(Entity* entity : item->rule->keys){
            if(item->dot == i){
                std::cout << ".";
            }else if(i>0){
                std::cout << " ";
            }
            printEntity(entity);
            i++;
        }
        if(item->dot ==i){
            std::cout << ".";
        }

        std::cout << " / ";

        for(Entity* entity : item->lookahead){
            printEntity(entity);

            std::cout << ", ";
        }

        //std::cout << "   ||||   " << item->hash();
        std::cout << std::endl;
    }

    void printClosure(Closure* closure){
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

    void printParserTable(ParserTable* table){
        for(uint32_t i = 0 ; i < table->entries[0]->size ; i++){
            std::cout << std::setw(10) << i;
        }
        std::cout << std::setw(10) << "";
        for(uint32_t i = 0 ; i < table->entries[0]->size ; i++){
            std::cout << std::setw(10) << i;
        }
        std::cout << std::setw(10) << "";
        for(uint32_t i = 0 ; i < table->entries[0]->size ; i++){
            std::cout << std::setw(10) << i;
        }
        std::cout << std::endl;

        for(uint32_t i = 0 ; i < table->size ; i++){
            Entry* entry = table->entries[i];
            printParserEntry(entry);
        }
    }

    void printParserEntry(Entry* entry){
        uint32_t width = 10;
        for(uint32_t i = 0 ; i < entry->size ; i++){
            std::cout << std::setw(width) << entry->actions[i];
        }
        std::cout << std::setw(width) << "";
        for(uint32_t i = 0 ; i < entry->size ; i++){
            std::cout << std::setw(width);
            if(entry->restore[i] != nullptr){
                printRule(entry->restore[i]);
            }else{
                std::cout <<  "";
            }
        }
        std::cout << std::setw(width) << "";
        for(uint32_t i = 0 ; i < entry->size ; i++){
            std::cout << std::setw(width) << entry->jumps[i];
        }
        std::cout << std::setw(width) << "";
        std::cout << std::endl;
    }


    void init(Grammar* grammar){
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
            auto* entry = new Entry(grammar->entities.size());
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

            printClosure(pair.second);
        }

        printParserTable(table);

        return table;
    }
};
