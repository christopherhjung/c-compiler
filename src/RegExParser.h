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

#include "InputReader.h"
#include "BinaryNode.h"
#include "UnaryNode.h"
#include "ValueNode.h"


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
    unordered_map<char, State*> transitions;
};

class RegExParser {
public:
    InputReader reader;
    bool nullifiesLeft = false;
    bool nullifiesRight = false;
    int currentIndex = 0;
    Info* finish;
    unordered_map<int, Info*> infos;
    unordered_map<int, char> values;
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

    State* parse()
    {
        finish = parseOr();
        return compile(finish->firstPositions);
    }

    State* compile(unordered_set<int> set)
    {
        if(states.find(set) != states.end()){
            return states[set];
        }

        bool isFinish = false;
        unordered_map<char, unordered_set<int>> next;
        for (int key : set)
        {
            if (finish->index == key)
            {
                isFinish = true;
            }
            else
            {
                char value = values[key];
                if(next.find(value) == next.end()){
                    next.insert(pair<char, unordered_set<int>>(value, unordered_set<int>()));
                }

                unordered_set<int>& test = next[value];

                if(infos.find(key) != infos.end()){
                    Info* info = infos[key];
                    test.insert(info->followPositions.begin(), info->followPositions.end());
                }
            }
        }

        State* state = new State();
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

                result->lastPositions.insert(left->firstPositions.begin(), left->firstPositions.end());
                result->lastPositions.insert(right->firstPositions.begin(), right->firstPositions.end());

                return result;
            }
        }

        return left;
    }

    Info* parseConcat()
    {
        Info* state = parseMultiplier();

        if (hasNext())
        {
            if (!is('|') && !is(')'))
            {
                Info* rightState = parseConcat();
                Info* result = createInfo();

                result->firstPositions.insert(state->firstPositions.begin(), state->firstPositions.end());
                result->lastPositions.insert(state->lastPositions.begin(), state->lastPositions.end());

                if (state->nullifies)
                {
                    result->firstPositions.insert(rightState->firstPositions.begin(), rightState->firstPositions.end());
                }

                if (rightState->nullifies)
                {
                    result->lastPositions.insert(state->firstPositions.begin(), state->firstPositions.end());
                }

                for(int lastPosition : state->lastPositions)
                {
                    infos[lastPosition]->followPositions.insert(rightState->firstPositions.begin(), rightState->firstPositions.end());
                }

                result->nullifies = state->nullifies && rightState->nullifies;

                return result;
            }
        }

        return state;
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
                //error
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
                char c = eat();

                Info* result = createInfo();
                result->nullifies = false;

                result->firstPositions.insert(result->index);
                result->lastPositions.insert(result->index);

                values.insert(pair<int, char>(result->index, c));

                return result;
            }
        }

        return 0;
    }
};


#endif //C4_REGEXPARSER_H
