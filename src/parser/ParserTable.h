//
// Created by Christopher Jung on 20.11.20.
//

#pragma once

#include "Grammar.h"
#include "Component.h"
#include "Entity.h"
#include "Rule.h"

class Entry{
public:
    int32_t* actions;
    int32_t* jumps;
    Rule** restore;
    uint32_t terminalSize;
    uint32_t nonTerminalSize;

    Entry(Grammar* grammar){
        terminalSize = grammar->terminals.size();
        nonTerminalSize = grammar->nonTerminals.size();

        actions = new int32_t[terminalSize];
        restore = new Rule*[terminalSize];
        jumps = new int32_t[nonTerminalSize];

        std::fill(actions, actions + terminalSize , -1);
        std::fill(jumps, jumps + nonTerminalSize , -1);
        std::fill(restore, restore + terminalSize , nullptr);
    }
};

class ParserTable {
public:
    Entry** entries;
    uint32_t size;

    ParserTable(uint32_t size) : size(size){
        entries = new Entry*[size];
    }
};
