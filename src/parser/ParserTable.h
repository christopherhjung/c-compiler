//
// Created by Christopher Jung on 20.11.20.
//

#pragma once



class Entry{
public:
    int32_t* actions;
    int32_t* jumps;
    Rule** restore;
    uint32_t size;

    Entry(uint32_t size) : size(size){
        actions = new int32_t[size];
        jumps = new int32_t[size];
        restore = new Rule*[size];

        std::fill(actions, actions + size , -1);
        std::fill(jumps, jumps + size , -1);
        std::fill(restore, restore + size , nullptr);
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
