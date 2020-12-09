//
// Created by Christopher Jung on 20.11.20.
//

#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <memory>
#include <algorithm>
#include <functional>
#include <cctype>
#include <locale>
#include <vector>
#include <unordered_map>
#include <unordered_set>

struct State{
public:
    bool finish = false;
    bool greedy = false;
    std::string name;
    int32_t index = -1;
    int32_t id = -1;
    State(bool finish, uint32_t index ,uint32_t id, std::string& name) : finish(finish), name(name), index(index), id(id) {};
    State(bool finish, uint32_t index ) : finish(finish), index(index){};
    std::unordered_map<char, State*> transitions;
};

class StateMachine {
public:
    State* root;
    int32_t eof = -1;
    std::vector<State*> states;
    std::vector<std::string> kinds;
    std::vector<bool> hides;
    std::vector<bool> catches;

    StateMachine(State *init, int32_t eof, const std::vector<State *> &states, const std::vector<std::string> &kinds,
                 const std::unordered_set<uint32_t>& hides,const std::unordered_set<uint32_t>& catches) :
            root(init), eof(eof), states(states), kinds(kinds)
    {

        this->hides.resize(kinds.size());
        std::fill(this->hides.begin() , this->hides.end(), false);
        for( const uint32_t& id : hides){
            this->hides[id] = true;
        }


        this->catches.resize(kinds.size());
        std::fill(this->catches.begin() , this->catches.end(), false);
        for( const uint32_t& id : catches){
            this->catches[id] = true;
        }
    }
};
