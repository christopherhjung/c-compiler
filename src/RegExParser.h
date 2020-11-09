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
#include "InputReader.h"

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
    InputReader reader;
    int currentIndex = 0;
    Info* finish;
    unordered_map<int, Info*> infos;
    unordered_map<int, vector<char>> values;
    unordered_map<int, string> finalState;
    unordered_map<unordered_set<int>, State*, SetHash> states;

    RegExParser(ifstream *stream) : reader(stream){
        reader.next();
    }

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
        infos.insert(pair<int,Info*>(info->index,info));
        return info;
    }

    void add(const string& name, const string& regex)
    {
        finish = parseOr();
        Info* last = createInfo();

        if (finish->nullifies)
        {
            finish->firstPositions.insert(last->index);
        }

        for (int lastPosition : finish->lastPositions)
        {
            infos[lastPosition]->followPositions.insert(last->index);
        }

        finalState.insert(pair<int, string>(last->index, name));
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
        for (int key : set)
        {
            if (finalState.find(key) != finalState.end())
            {
                isFinish = true;
                name = finalState[key];
            }
            else
            {
                for(char value : values[key]){
                    if(next.find(value) == next.end()){
                        next.insert(pair<char, unordered_set<int>>(value, unordered_set<int>()));
                    }

                    unordered_set<int>& nextSet = next[value];

                    if(infos.find(key) != infos.end()){
                        Info* info = infos[key];
                        nextSet.insert(info->followPositions.begin(), info->followPositions.end());
                    }
                }
            }
        }

        State* state = new State(isFinish, name);
        states.insert(pair<unordered_set<int>, State*>(set, state));
        for (pair<char, unordered_set<int>> element : next)
        {
            State* following = compile(element.second);
            state->transitions.insert(pair<char, State*>(element.first, following));
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
            int type = -1;
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

                vector<char> vec;
                computeValue(vec);
                values.insert(pair<int, vector<char>>(result->index, vec));

                return result;
            }
        }

        return 0;
    }

    void computeValue(vector<char>& vec){
        char specifier = eat();
        if(specifier == '.')
        {
            for(int i = 0 ; i < 127 ; i++){
                if(i == '\n')
                {
                    continue;
                }

                char c = (char) i;

                vec.push_back(c);
            }
        }else if(specifier == '\\'){
            computeEscaped(eat(), vec);
        }
        else{
            vec.push_back(specifier);
        }
    }

    static void computeEscaped(char specifier ,vector<char>& vec){
        if (specifier == 'n')
        {
            return vec.push_back('\n');
        }
        else if (specifier == 'd')
        {
            for(int i = 0 ; i < 127 ; i++){
                if(i == '\n')
                {
                    continue;
                }

                char c = (char) i;

                vec.push_back(c);
            }
        }
        else if (specifier == 's')
        {
            vec.push_back('\n');
            vec.push_back(' ');
            vec.push_back('\t');
            vec.push_back('\r');
        }
        else if (specifier == 'w')
        {
            addRange('a', 'z', vec);
            addRange('A', 'Z', vec);
            addRange('a', 'z', vec);
            vec.push_back('_');
        }
        else
        {
            return vec.push_back(specifier);
        }
    }

    static void addRange(char from, char to, vector<char>& vec){
        for( ; from <= to; from++ ){
            vec.push_back(from);
        }
    }
};


#endif //C4_REGEXPARSER_H
