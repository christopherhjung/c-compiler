//
// Created by Christopher Jung on 22.11.20.
//

#pragma once

#include <iostream>
#include <iomanip>

void printEntity(const Entity* item){
    std::cout << item->name;
}

void printRule(const Rule* rule){
    printEntity(rule->owner);

    std::cout << "->";
    uint32_t i = 0;
    for(Entity* entity : rule->keys){
        std::cout << " ";
        printEntity(entity);
        i++;
    }
}

void printItem(const Item* item){
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


void printParserEntry(Entry* entry){
    uint32_t width = 10;
    for(uint32_t i = 0 ; i < entry->terminalSize ; i++){
        std::cout << std::setw(width) << entry->actions[i];
    }
    std::cout << std::setw(width) << "";
    for(uint32_t i = 0 ; i < entry->terminalSize ; i++){
        std::cout << std::setw(width);
        if(entry->restore[i] != nullptr){
            printRule(entry->restore[i]);
        }else{
            std::cout <<  "";
        }
    }
    std::cout << std::setw(width) << "";
    for(uint32_t i = 0 ; i < entry->nonTerminalSize ; i++){
        std::cout << std::setw(width) << entry->jumps[i];
    }
    std::cout << std::setw(width) << "";
    std::cout << std::endl;
}

void printParserTable(ParserTable* table){
    for(uint32_t i = 0 ; i < table->entries[0]->terminalSize ; i++){
        std::cout << std::setw(10) << i;
    }
    std::cout << std::setw(10) << "";
    for(uint32_t i = 0 ; i < table->entries[0]->terminalSize ; i++){
        std::cout << std::setw(10) << i;
    }
    std::cout << std::setw(10) << "";
    for(uint32_t i = 0 ; i < table->entries[0]->nonTerminalSize ; i++){
        std::cout << std::setw(10) << i;
    }
    std::cout << std::endl;

    for(uint32_t i = 0 ; i < table->size ; i++){
        Entry* entry = table->entries[i];
        printParserEntry(entry);
    }
}

