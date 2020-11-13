//
// Created by Christopher Jung on 2020-11-06.
//

#pragma once

#include <iostream>
#include <fstream>
#include <string>

#include <unordered_set>
#include <unordered_map>
#include <utility>
#include <vector>
#include "../reader/StringInputReader.h"

#define CHAR_COUNT 256

struct SetHash {
    size_t operator()(const std::unordered_set<uint32_t>& s) const {
        size_t sum{31};
        for ( int e : s )
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
    uint32_t id;
    State(bool finish, uint32_t id, std::string name) : finish(finish), id(id), name(std::move(name)){};
    State(bool finish) : finish(finish){};
    //std::unordered_map<char, State*> transitions;
    State* transitions[CHAR_COUNT]{nullptr};
};

class StateMachineBuilder {
public:
    StringInputReader reader;
    uint32_t currentIndex = 0;
    uint32_t currentRule = 0;
    Info* finish = nullptr;
    std::unordered_map<uint32_t, Info*> infos;

    std::unordered_map<char, char> escaping = {{'a', '\a'}, {'b', '\b'}, {'f', '\f'},{'n', '\n'},{'r', '\r'},{'t', '\t'},{'v', '\v'}};
    std::unordered_map<uint32_t, std::unique_ptr<bool>> values;
    std::unordered_map<uint32_t, int> rules;
    std::unordered_map<uint32_t, std::string> kinds;
    std::unordered_map<std::unordered_set<uint32_t>, State*, SetHash> states;

    virtual ~StateMachineBuilder() {
        for(const auto& infoEntry : infos){
            delete infoEntry.second;
        }
    }

    char eat(){
        char result = reader.peek();
        if(hasNext()){
            reader.next();
        }
        return result;
    }

    bool eat(char cha){
        if(reader.hasCurrent() && reader.peek() == cha){
            reader.next();
            return true;
        }
        return false;
    }

    bool is(char cha){
        return reader.hasCurrent() && reader.peek() == cha;
    }

    bool hasNext(){
        return reader.hasCurrent();
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
        kinds[rule] = name;

        if(finish != nullptr){
            finish = wrapOr(finish, newInfo);
        }else{
            finish = newInfo;
        }
    }

    std::unique_ptr<State> build(){
        states.clear();
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
        int32_t rule = -1;
        for (uint32_t key : set)
        {
            if (rules.find(key) != rules.end())
            {
                isFinish = true;
                if(rule == -1 || key < rule){
                    rule = rules[key];
                }
            }
            else
            {
                auto& arr = values[key];
                for(uint32_t value = 0; value < CHAR_COUNT; value++){
                    if(arr.get()[value]){
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
        if(rule == -1){
            state = new State(isFinish);
        }else{
            state = new State(isFinish, rule, kinds[rule]);
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

                values[result->index] = std::unique_ptr<bool>(computeValue());

                return result;
            }
        }

        return nullptr;
    }

    bool* computeValue(){
        bool* arr = new bool[CHAR_COUNT]{false};

        char specifier = eat();
        if(specifier == '.')
        {
            for(uint32_t i = 0 ; i < CHAR_COUNT ; i++){
                arr[i] = true;
            }

            arr['\n'] = false;

        }
        else if (specifier == '[')
        {
            bool negate = eat('^');

            int32_t save = -1;

            while (true)
            {
                if (save >= 0 && eat('-'))
                {
                    char end;
                    if (is('\\'))
                    {
                        throw std::exception();
                    }

                    addRange((char)save, eat(), arr);
                    save = -1;
                }
                else
                {
                    if (save >= 0)
                    {
                        arr[save] = true;
                    }

                    if (eat(']'))
                    {
                        break;
                    }

                    if (eat('\\'))
                    {
                        computeEscaped(arr);
                    }
                    else
                    {
                        save = eat();
                    }
                }
            }

            if (negate)
            {
                for(uint32_t i = 0 ; i < CHAR_COUNT ; i++){
                    arr[i] ^= true;
                }
            }
        }
        else if(specifier == '\\'){
            computeEscaped( arr);
        }
        else{
            arr[specifier] = true;
        }

        return arr;
    }


    void computeEscaped(bool* arr){
        char specifier = eat();
        auto result = escaping.find(specifier);
        if (result != escaping.end())
        {
            arr[result->second] = true;
        }
        else if (specifier == 's')
        {
            arr['\n'] = true;
            arr[' '] = true;
            arr['\t'] = true;
            arr['\r'] = true;
        }
        else if (specifier == 'w')
        {
            addRange('a', 'z', arr);
            addRange('A', 'Z', arr);
            addRange('0', '9', arr);
            arr['_'] = true;
        }
        else
        {
            arr[specifier] = true;
        }
    }

    static void addRange(char from, char to, bool* vec){
        for( ; from <= to; from++ ){
            vec[from] = true;
        }
    }
};
