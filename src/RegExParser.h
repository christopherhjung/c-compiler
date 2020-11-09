//
// Created by Christopher Jung on 2020-11-06.
//

#ifndef C4_REGEXPARSER_H
#define C4_REGEXPARSER_H

#include <iostream>
#include <fstream>
#include <string>

#include <unordered_set>
#include <unordered_map>
#include <vector>
#include "StringInputReader.h"

using namespace std;

struct SetHash {
    size_t operator()(const unordered_set<int>& s) const {
        size_t sum{31};
        for ( int e : s )
            sum *= e;
        return sum;
    }
};

class Info{
public:
    int index;
    bool nullifies;
    unordered_set<int> firstPositions;
    unordered_set<int> lastPositions;
    unordered_set<int> followPositions;
};

class State{
public:
    bool finish;
    string name;
    State(bool finish, string name) : finish(finish), name(name){};
    unordered_map<char, State*> transitions;
};

class RegExParser {
public:
    StringInputReader reader;
    int currentIndex = 0;
    Info* finish = nullptr;
    unordered_map<int, Info*> infos;
    unordered_map<int, bool*> values;
    unordered_map<int, string> finalState;
    unordered_map<unordered_set<int>, State*, SetHash> states;

    char eat(){
        char result = reader.peek();
        if(hasNext()){
            reader.next();
        }
        return result;
    }

    bool eat(char cha){
        if(reader.hasNext() && reader.peek() == cha){
            reader.next();
            return true;
        }
        return false;
    }

    bool is(char cha){
        return reader.hasNext() && reader.peek() == cha;
    }

    bool hasNext(){
        return reader.hasNext();
    }

    Info* createInfo(){
        Info* info = new Info();
        info->index = currentIndex++;
        //infos.insert(pair<int,Info*>(info->index,info));
        infos[info->index] = info;
        return info;
    }

    void add(const string& name, const string& regex)
    {
        reader.reset(regex);
        Info* newInfo = parseOr();
        //Info* last = createInfo();
        int finalIndex = currentIndex++;

        if (newInfo->nullifies)
        {
            newInfo->firstPositions.insert(finalIndex);
        }

        for (int lastPosition : newInfo->lastPositions)
        {
            infos[lastPosition]->followPositions.insert(finalIndex);
        }

        //finalState.insert(pair<int, string>(last->index, name));
        finalState[finalIndex] = name;

        if(finish != nullptr){
            Info* left = finish;
            Info* right = newInfo;

            Info* result = createInfo();
            result->nullifies = left->nullifies || right->nullifies;

            result->firstPositions.insert(left->firstPositions.begin(), left->firstPositions.end());
            result->firstPositions.insert(right->firstPositions.begin(), right->firstPositions.end());

            result->lastPositions.insert(left->lastPositions.begin(), left->lastPositions.end());
            result->lastPositions.insert(right->lastPositions.begin(), right->lastPositions.end());

            finish = result;
        }else{
            finish = newInfo;
        }
    }

    State* build(){
        return compile(finish->firstPositions);
    }

    State* compile(const unordered_set<int>& set)
    {
        if(states.find(set) != states.end()){
            return states[set];
        }

        bool isFinish = false;
        unordered_map<char, unordered_set<int>> next;
        string name;
        int usedKey = -1;
        for (int key : set)
        {
            if (finalState.find(key) != finalState.end())
            {
                isFinish = true;
                if(usedKey == -1 || key < usedKey){
                    name = finalState[key];
                    usedKey = key;
                }
            }
            else
            {
                bool* arr = values[key];
                for(int value = 0; value < 128; value++){
                    if(arr[value]){
                        if(next.find(value) == next.end()){
                            //next.insert(pair<char, unordered_set<int>>(value, unordered_set<int>()));
                            next[value] = unordered_set<int>();
                        }

                        unordered_set<int>& nextSet = next[value];

                        if(infos.find(key) != infos.end()){
                            Info* info = infos[key];
                            nextSet.insert(info->followPositions.begin(), info->followPositions.end());
                        }
                    }
                }
            }
        }

        State* state = new State(isFinish, name);
        //states.insert(pair<unordered_set<int>, State*>(set, state));
        states[set] = state;
        for (pair<char, unordered_set<int>> element : next)
        {
            State* following = compile(element.second);
            //state->transitions.insert(pair<char, State*>(element.first, following));
            state->transitions[element.first] = following;
        }

        return state;
    }

    Info* parseOr()
    {
        Info* left = parseConcat();

        if (hasNext())
        {
            if (eat('|'))
            {
                Info* right = parseOr();
                Info* result = createInfo();
                result->nullifies = left->nullifies || right->nullifies;

                result->firstPositions.insert(left->firstPositions.begin(), left->firstPositions.end());
                result->firstPositions.insert(right->firstPositions.begin(), right->firstPositions.end());

                result->lastPositions.insert(left->lastPositions.begin(), left->lastPositions.end());
                result->lastPositions.insert(right->lastPositions.begin(), right->lastPositions.end());

                return result;
            }
        }

        return left;
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
            int type;
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
                for (int child : state->lastPositions)
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
                    //exit
                    //throw new RuntimeException("No Parenthesis closing!");
                }

                return state;
            }
            else
            {
                Info* result = createInfo();
                result->nullifies = false;

                result->firstPositions.insert(result->index);
                result->lastPositions.insert(result->index);

                bool* arr = computeValue();
                values[result->index] = arr;

                return result;
            }
        }

        return nullptr;
    }

    bool* computeValue(){
        bool* arr = new bool[128]{false};

        char specifier = eat();
        if(specifier == '.')
        {
            for(int i = 0 ; i < 127 ; i++){
                arr[i] = true;
            }

            arr['\n'] = false;

        }
        else if (specifier == '[')
        {
            bool negate = eat('^');

            int save = -1;

            while (true)
            {
                if (save >= 0 && eat('-'))
                {
                    if (is('\\'))
                    {
                        //error
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
                for(int i = 0 ; i < 128 ; i++){
                    arr[i] ^= !arr[i];
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
        if (specifier == 'n')
        {
            arr['\n'] = true;
        }
        else if (specifier == 'd')
        {
            addRange('0', '9', arr);
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


#endif //C4_REGEXPARSER_H
