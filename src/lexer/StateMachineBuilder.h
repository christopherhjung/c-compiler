//
// Created by Christopher Jung on 2020-11-06.
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

#include <unordered_set>
#include <unordered_map>
#include <utility>
#include <vector>
#include "../reader/StringInputReader.h"
#include "StateMachine.h"
#include <regex>
#include "LexerGrammar.h"

#define CHAR_COUNT 256


struct SetHash {
    size_t operator()(const std::unordered_set<uint32_t> &s) const {
        size_t sum{31};
        for (auto &e : s)
            sum *= e;
        return sum;
    }
};

struct Info {
public:
    uint32_t index;
    uint32_t rule;
    bool nullifies;
    std::unordered_set<uint32_t> firstPositions;
    std::unordered_set<uint32_t> lastPositions;
    std::unordered_set<uint32_t> followPositions;
};

class StateMachineBuilder {
public:
    StringInputReader reader;
    uint32_t currentIndex = 0;
    uint32_t currentRule = 0;
    uint32_t stateIndex = 0;
    int32_t eof = -1;
    Info *finish = nullptr;
    std::unordered_map<uint32_t, Info *> infos;

    std::unordered_map<char, char> escaping = {{'0', '\0'},
                                               {'a', '\a'},
                                               {'b', '\b'},
                                               {'f', '\f'},
                                               {'n', '\n'},
                                               {'r', '\r'},
                                               {'t', '\t'},
                                               {'v', '\v'}};
    std::unordered_map<uint32_t, std::unordered_set<char>> values;
    std::unordered_map<uint32_t, uint32_t> rules;
    std::vector<std::string> kinds;
    std::unordered_set<uint32_t> greedys;
    std::unordered_set<uint32_t> hides;
    std::unordered_set<uint32_t> catches;
    std::unordered_map<std::unordered_set<uint32_t>, State *, SetHash> states;

    static StateMachine *build(InputReader *inputReader) {
        LexerGrammar grammar = LexerGrammar::build(inputReader);

        return build(grammar);
    }

    static StateMachine *build(LexerGrammar grammar) {
        StateMachineBuilder builder;
        for (const auto &entry : grammar.entries) {
            builder.add(entry.name, entry.regex, entry.greedy, entry.hide, entry.catchValue);
        }
        return builder.build();
    }

    virtual ~StateMachineBuilder() {
        for (const auto &infoEntry : infos) {
            delete infoEntry.second;
        }
    }

    std::vector<State *> getStates() {
        std::vector<State *> result;
        for (const auto &pair : states) {
            result.push_back(pair.second);
        }
        return result;
    }

    std::vector<std::string> getKinds() {
        return kinds;
    }

    char eat() {
        char result = reader.peek();
        if (hasNext()) {
            reader.next();
        }
        return result;
    }

    bool eat(char cha) {
        if (reader.peek() == cha) {
            reader.next();
            return true;
        }
        return false;
    }

    bool is(char cha) {
        return reader.peek() == cha;
    }

    bool hasNext() {
        return reader.peek() != 256;
    }

    Info *createInfo() {
        Info *info = new Info();
        info->index = currentIndex++;
        info->rule = currentRule;
        infos[info->index] = info;
        return info;
    }

    void add(const std::string &name, const std::string &regex, bool greedy, bool hide, bool catchValue) {
        if (name == "eof") {
            eof = currentRule;
        } else {
            reader.reset(regex);
            Info *newInfo = parseOr();
            Info *endInfo = createInfo();
            int finalIndex = endInfo->index;

            if (newInfo->nullifies) {
                newInfo->firstPositions.insert(finalIndex);
            }

            for (int lastPosition : newInfo->lastPositions) {
                infos[lastPosition]->followPositions.insert(finalIndex);
            }

            if (greedy) {
                greedys.insert(endInfo->rule);
            }

            if (hide) {
                hides.insert(endInfo->rule);
            }

            if (catchValue) {
                catches.insert(endInfo->rule);
            }
            rules[finalIndex] = endInfo->rule;

            if (finish != nullptr) {
                finish = wrapOr(finish, newInfo);
            } else {
                finish = newInfo;
            }
        }

        kinds.push_back(name);
        currentRule++;
    }

    StateMachine *build() {
        states.clear();
        stateIndex = 0;
        State *root = compile(finish->firstPositions);

        return new StateMachine(root, eof, getStates(), kinds, hides, catches);
    }

    State *compile(const std::unordered_set<uint32_t> &set) {
        if (states.find(set) != states.end()) {
            return states[set];
        }

        bool isFinish = false;
        std::unordered_map<char, std::unordered_set<uint32_t>> followingStates;
        std::unordered_map<char, std::unordered_set<uint32_t>> followingRules;
        uint32_t rule = 0;
        for (uint32_t key : set) {
            if (rules.find(key) != rules.end()) {
                if (!isFinish || rules[key] < rule) {
                    rule = rules[key];
                }
                isFinish = true;
            } else {
                for (const char &value : values[key]) {
                    followingStates[value].insert(key);
                    Info *info = infos[key];
                    followingRules[value].insert(info->rule);
                }
            }
        }

        State *state;
        if (isFinish) {
            state = new State(isFinish, stateIndex++, rule, kinds[rule]);
        } else {
            state = new State(isFinish, stateIndex++);
        }
        states[set] = state;
        for (const auto &element : followingStates) {
            std::unordered_set<uint32_t> &followingRule = followingRules[element.first];
            State *following;
            if (element.second.size() == 1) {
                uint32_t infoKey = *element.second.begin();
                Info *info = infos[infoKey];
                following = compile(info->followPositions);
            } else {
                std::unordered_set<uint32_t> followingPos;
                for (const auto &infoKey : element.second) {
                    Info *info = infos[infoKey];
                    followingPos.insert(info->followPositions.begin(), info->followPositions.end());
                }
                following = compile(followingPos);
            }

            if (followingRule.size() == 1) {
                uint32_t singleRule = *followingRule.begin();
                if (greedys.find(singleRule) != greedys.end()) {
                    following->greedy = true;
                }
            }

            state->transitions[element.first] = following;
        }

        return state;
    }

    Info *parseOr() {
        Info *left = parseConcat();

        if (eat('|')) {
            return wrapOr(left, parseOr());
        }

        return left;
    }

    Info *wrapOr(Info *left, Info *right) {
        Info *result = createInfo();
        result->nullifies = left->nullifies || right->nullifies;

        result->firstPositions.insert(left->firstPositions.begin(), left->firstPositions.end());
        result->firstPositions.insert(right->firstPositions.begin(), right->firstPositions.end());

        result->lastPositions.insert(left->lastPositions.begin(), left->lastPositions.end());
        result->lastPositions.insert(right->lastPositions.begin(), right->lastPositions.end());
        return result;
    }

    Info *parseConcat() {
        Info *leftState = parseMultiplier();

        if (hasNext()) {
            if (!is('|') && !is(')')) {
                Info *rightState = parseConcat();
                Info *result = createInfo();

                result->firstPositions.insert(leftState->firstPositions.begin(), leftState->firstPositions.end());
                result->lastPositions.insert(rightState->lastPositions.begin(), rightState->lastPositions.end());

                if (leftState->nullifies) {
                    result->firstPositions.insert(rightState->firstPositions.begin(), rightState->firstPositions.end());
                }

                if (rightState->nullifies) {
                    result->lastPositions.insert(leftState->lastPositions.begin(), leftState->lastPositions.end());
                }

                for (uint32_t lastPosition : leftState->lastPositions) {
                    infos[lastPosition]->followPositions.insert(rightState->firstPositions.begin(),
                                                                rightState->firstPositions.end());
                }

                result->nullifies = leftState->nullifies && rightState->nullifies;

                return result;
            }
        }

        return leftState;
    }

    Info *parseMultiplier() {
        Info *state = parseParenthesis();

        if (hasNext()) {
            uint8_t type;
            if (eat('*')) {
                type = 1;
            } else if (eat('?')) {
                type = 0;
            } else if (eat('+')) {
                type = 2;
            } else {
                return state;
            }

            Info *result = createInfo();

            result->firstPositions.insert(state->firstPositions.begin(), state->firstPositions.end());
            result->lastPositions.insert(state->lastPositions.begin(), state->lastPositions.end());

            result->nullifies = type != 2;

            if (type != 0) {
                for (uint32_t child : state->lastPositions) {
                    infos[child]->followPositions.insert(result->firstPositions.begin(), result->firstPositions.end());
                }
            }

            return result;
        }

        return state;
    }

    Info *parseParenthesis() {
        if (hasNext()) {
            if (eat('(')) {
                Info *state = parseOr();
                if (!eat(')')) {
                    throw std::exception();
                }

                return state;
            } else {
                Info *result = createInfo();
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

    void computeValue(std::unordered_set<char> &set) {
        char specifier = eat();
        if (specifier == '.') {
            for (uint32_t i = 0; i < CHAR_COUNT; i++) {
                if (i != '\n') {
                    set.insert(i);
                }
            }
        } else if (specifier == '[') {
            bool negate = eat('^');

            int32_t save = -1;

            while (true) {
                if (save >= 0 && eat('-')) {
                    if (is('\\')) {
                        throw std::exception();
                    }

                    addRange((char) save, eat(), set);
                    save = -1;
                } else {
                    if (save >= 0) {
                        set.insert(save);
                    }

                    if (eat(']')) {
                        break;
                    }

                    if (eat('\\')) {
                        computeEscaped(set);
                    } else {
                        save = eat();
                    }
                }
            }

            for (uint32_t i = 0; i < CHAR_COUNT; i++) {
                if ((set.find(i) != set.end()) ^ negate) {
                    set.insert(i);
                } else {
                    set.erase(i);
                }
            }
        } else if (specifier == '\\') {
            computeEscaped(set);
        } else {
            set.insert(specifier);
        }
    }


    void computeEscaped(std::unordered_set<char> &set) {
        char specifier = eat();
        auto result = escaping.find(specifier);
        if (result != escaping.end()) {
            set.insert(result->second);
        } else if (specifier == 's') {
            set.insert('\n');
            set.insert(' ');
            set.insert('\t');
            set.insert('\r');
        } else if (specifier == 'w') {
            addRange('a', 'z', set);
            addRange('A', 'Z', set);
            addRange('0', '9', set);
            set.insert('_');
        } else {
            set.insert(specifier);
        }
    }

    static void addRange(char from, char to, std::unordered_set<char> &set) {
        for (; from <= to; from++) {
            set.insert(from);
        }
    }
};
