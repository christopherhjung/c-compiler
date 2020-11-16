//
// Created by Christopher Jung on 2020-11-06.
//

#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <memory>

#include <unordered_set>
#include <unordered_map>
#include <utility>
#include <vector>
#include "../reader/StringInputReader.h"

#define CHAR_COUNT 256

struct SetHash {
    size_t operator()(const std::unordered_set<uint32_t>& s) const {
        size_t sum{31};
        for ( auto& e : s )
            sum *= e;
        return sum;
    }
};

struct Info{
public:
    int index;
    bool nullifies;
    std::unordered_set<uint32_t> firstPositions;
    std::unordered_set<uint32_t> lastPositions;
    std::unordered_set<uint32_t> followPositions;
};

struct State{
public:
    bool finish;
    std::string name;
    int32_t index = -1;
    int32_t id = -1;
    State(bool finish, uint32_t index ,uint32_t id, std::string& name) : finish(finish), name(name), index(index), id(id) {};
    State(bool finish, uint32_t index ) : finish(finish), index(index){};
    std::unordered_map<char, State*> transitions;
    //State* transitions[CHAR_COUNT]{nullptr};

};

class StateMachineBuilder {
public:
    StringInputReader reader;
    uint32_t currentIndex = 0;
    uint32_t currentRule = 0;
    uint32_t stateIndex = 0;
    Info* finish = nullptr;
    std::unordered_map<uint32_t, Info*> infos;

    std::unordered_map<char, char> escaping = {{'a', '\a'}, {'b', '\b'}, {'f', '\f'},{'n', '\n'},{'r', '\r'},{'t', '\t'},{'v', '\v'}};
    std::unordered_map<uint32_t, std::unordered_set<char>> values;
    std::unordered_map<uint32_t, uint32_t> rules;
    std::vector<std::string> kinds;
    std::unordered_map<std::unordered_set<uint32_t>, State*, SetHash> states;

    virtual ~StateMachineBuilder() {
        for(const auto& infoEntry : infos){
            delete infoEntry.second;
        }
    }

    std::vector<State*> getStates(){
        std::vector<State*> result;
        for(const auto& pair : states){
            result.push_back(pair.second);
        }
        return result;
    }

    std::vector<std::string> getKinds(){
        return kinds;
    }

    char eat(){
        char result = reader.peek();
        if(hasNext()){
            reader.next();
        }
        return result;
    }

    bool eat(char cha){
        if(reader.peek() == cha){
            reader.next();
            return true;
        }
        return false;
    }

    bool is(char cha){
        return reader.peek() == cha;
    }

    bool hasNext(){
        return reader.peek() != 0;
    }

    Info* createInfo(){
        Info* info = new Info();
        info->index = currentIndex++;
        infos[info->index] = info;
        return info;
    }

    void add(const std::string& name, const std::string& regex)
    {
        reader.reset(regex);
        Info* newInfo = parseOr();
        int finalIndex = currentIndex++;

        if (newInfo->nullifies)
        {
            newInfo->firstPositions.insert(finalIndex);
        }

        for (int lastPosition : newInfo->lastPositions)
        {
            infos[lastPosition]->followPositions.insert(finalIndex);
        }

        int rule = currentRule++;
        rules[finalIndex] = rule;
        kinds.push_back(name);

        if(finish != nullptr){
            finish = wrapOr(finish, newInfo);
        }else{
            finish = newInfo;
        }
    }

    std::unique_ptr<State> build(){
        states.clear();
        stateIndex = 0;
        return std::unique_ptr<State>(compile(finish->firstPositions));
    }

    State* compile(const std::unordered_set<uint32_t>& set)
    {
        if(states.find(set) != states.end()){
            return states[set];
        }

        bool isFinish = false;
        std::unordered_map<char, std::unordered_set<uint32_t>> next;
        std::string name;
        uint32_t rule = 0;
        for (uint32_t key : set)
        {
            if (rules.find(key) != rules.end())
            {
                if(!isFinish || key < rule){
                    rule = rules[key];
                }
                isFinish = true;
            }
            else
            {
                auto& arr = values[key];
                for(uint32_t value = 0; value < CHAR_COUNT; value++){
                    if(arr.find(value) != arr.end()){
                        if(next.find(value) == next.end()){
                            next[value] = std::unordered_set<uint32_t>();
                        }

                        std::unordered_set<uint32_t>& nextSet = next[value];

                        if(infos.find(key) != infos.end()){
                            Info* info = infos[key];
                            nextSet.insert(info->followPositions.begin(), info->followPositions.end());
                        }
                    }
                }
            }
        }

        State* state;
        if(isFinish){
            state = new State(isFinish, stateIndex++, rule, kinds[rule]);
        }else{
            state = new State(isFinish , stateIndex++);
        }
        states[set] = state;
        for (const auto& element : next)
        {
            State* following = compile(element.second);
            state->transitions[element.first] = following;
        }

        return state;
    }

    Info* parseOr()
    {
        Info* left = parseConcat();

        if ( eat('|'))
        {
            return wrapOr(left, parseOr());
        }

        return left;
    }

    Info* wrapOr(Info* left, Info* right)
    {
        Info* result = createInfo();
        result->nullifies = left->nullifies || right->nullifies;

        result->firstPositions.insert(left->firstPositions.begin(), left->firstPositions.end());
        result->firstPositions.insert(right->firstPositions.begin(), right->firstPositions.end());

        result->lastPositions.insert(left->lastPositions.begin(), left->lastPositions.end());
        result->lastPositions.insert(right->lastPositions.begin(), right->lastPositions.end());
        return result;
    }

    Info* parseConcat()
    {
        Info* leftState = parseMultiplier();

        if (hasNext())
        {
            if (!is('|') && !is(')'))
            {
                Info* rightState = parseConcat();
                Info* result = createInfo();

                result->firstPositions.insert(leftState->firstPositions.begin(), leftState->firstPositions.end());
                result->lastPositions.insert(rightState->lastPositions.begin(), rightState->lastPositions.end());

                if (leftState->nullifies)
                {
                    result->firstPositions.insert(rightState->firstPositions.begin(), rightState->firstPositions.end());
                }

                if (rightState->nullifies)
                {
                    result->lastPositions.insert(leftState->lastPositions.begin(), leftState->lastPositions.end());
                }

                for(int lastPosition : leftState->lastPositions)
                {
                    infos[lastPosition]->followPositions.insert(rightState->firstPositions.begin(), rightState->firstPositions.end());
                }

                result->nullifies = leftState->nullifies && rightState->nullifies;

                return result;
            }
        }

        return leftState;
    }

    Info* parseMultiplier()
    {
        Info* state = parseParenthesis();

        if (hasNext())
        {
            uint8_t type;
            if (eat('*'))
            {
                type = 1;
            }
            else if (eat('?'))
            {
                type = 0;
            }
            else if (eat('+'))
            {
                type = 2;
            }else{
                return state;
            }

            Info* result = createInfo();

            result->firstPositions.insert(state->firstPositions.begin(), state->firstPositions.end());
            result->lastPositions.insert(state->lastPositions.begin(), state->lastPositions.end());

            result->nullifies = type != 2;

            if (type != 0)
            {
                for (uint32_t child : state->lastPositions)
                {
                    infos[child]->followPositions.insert(result->firstPositions.begin(), result->firstPositions.end());
                }
            }

            return result;
        }

        return state;
    }

    Info* parseParenthesis()
    {
        if (hasNext())
        {
            if (eat('('))
            {
                Info* state = parseOr();
                if (!eat(')'))
                {
                    throw std::exception();
                }

                return state;
            }
            else
            {
                Info* result = createInfo();
                result->nullifies = false;

                result->firstPositions.insert(result->index);
                result->lastPositions.insert(result->index);

                values[result->index] = std::unordered_set<char>();
                computeValue(values[result->index]);

                return result;
            }
        }

        return nullptr;
    }

    void computeValue(std::unordered_set<char>& set){
        char specifier = eat();
        if(specifier == '.')
        {
            for(uint32_t i = 1 ; i < CHAR_COUNT ; i++){
                if(i != '\n'){
                    set.insert(i);
                }
            }
        }
        else if (specifier == '[')
        {
            bool negate = eat('^');

            int32_t save = -1;

            while (true)
            {
                if (save >= 0 && eat('-'))
                {
                    if (is('\\'))
                    {
                        throw std::exception();
                    }

                    addRange((char)save, eat(), set);
                    save = -1;
                }
                else
                {
                    if (save >= 0)
                    {
                        set.insert(save);
                    }

                    if (eat(']'))
                    {
                        break;
                    }

                    if (eat('\\'))
                    {
                        computeEscaped(set);
                    }
                    else
                    {
                        save = eat();
                    }
                }
            }

            for(uint32_t i = 1 ; i < CHAR_COUNT ; i++){
                if((set.find(i) != set.end()) ^ negate){
                    set.insert(i);
                }else{
                    set.erase(i);
                }
            }
        }
        else if(specifier == '\\'){
            computeEscaped( set);
        }
        else{
            set.insert(specifier);
        }
    }


    void computeEscaped(std::unordered_set<char>& set){
        char specifier = eat();
        auto result = escaping.find(specifier);
        if (result != escaping.end())
        {
            set.insert(result->second);
        }
        else if (specifier == 's')
        {
            set.insert('\n');
            set.insert(' ');
            set.insert('\t');
            set.insert('\r');
        }
        else if (specifier == 'w')
        {
            addRange('a', 'z', set);
            addRange('A', 'Z', set);
            addRange('0', '9', set);
            set.insert('_');
        }
        else
        {
            set.insert(specifier);
        }
    }

    static void addRange(char from, char to, std::unordered_set<char>& set){
        for( ; from <= to; from++ ){
            set.insert(from);
        }
    }
};
