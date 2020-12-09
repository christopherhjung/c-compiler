//
// Created by Christopher Jung on 2020-11-06.
//

#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <memory>
#include <regex>
#include <unordered_set>
#include <unordered_map>
#include <utility>
#include <vector>
#include "../reader/StringInputReader.h"
#include "../parser/Grammar.h"
#include "../reader/FileInputReader.h"
#include "../lexer/Token.h"
#include "../lexer/StateMachineBuilder.h"
#include "../lexer/StateMachine.h"
#include "../lexer/StateMachineLexer.h"

#define EOF 0
#define PARSER_NAME 4
#define LEXER_NAME 5
#define LEXER_VALUE 6
#define QUESTION 7
#define LEFT 8
#define RIGHT 9
#define COLON 10
#define SEMI 11
#define OR 12
#define PLUS 13
#define STAR 14

struct Node{
    std::vector<std::vector<Entity*>> rules;
};

class ParserCodeGenerator {
public:
    LexerGrammar lexerGrammar;
    StringInputReader reader;
    StateMachineLexer stateMachineLexer;
    Token token;
    uint32_t nonTerminalId = 0;

    std::unordered_map<std::string, Entity*> terminals;
    std::unordered_map<std::string, Entity*> nonTerminals;

    ParserCodeGenerator(LexerGrammar lexerGrammar) : lexerGrammar(lexerGrammar), stateMachineLexer(StateMachineBuilder::build(new FileInputReader("./resources/astTree.lexer"))){
        uint32_t id = 0;
        std::regex pattern (R"(\\)");
        std::regex isRegex (R"([^\\][[\]\{\}\(\)\*\+])");
        for( const auto& entry : lexerGrammar.entries )
        {
            auto* entity = new Entity(id++, entry.name, true);
            terminals[entry.name] = entity;

            if(!std::regex_match(entry.regex, isRegex)){
                std::string alt = std::regex_replace(entry.regex , pattern, "");
                terminals[alt] = entity;
            }
        }
    }

    void init(InputReader* parserDescriptor){
        stateMachineLexer.reset(parserDescriptor);
        next();
    }

    void next(){
        stateMachineLexer.hasNextToken(token);
    }

    Entity* getEntity(){
        Entity* result = nullptr;
        switch (token.id){
            case PARSER_NAME:
                if(nonTerminals.find(*token.value) != nonTerminals.end()){
                    result = nonTerminals[*token.value];
                }else{
                    result = new Entity(nonTerminalId++,*token.value, false);
                    nonTerminals.emplace(token.value, result );
                }

                break;
            case LEXER_NAME:
            case LEXER_VALUE:
                std::string str = (*token.value).substr(1, (*token.value).size() - 2);
                if(terminals.find(str) == terminals.end()){
                    throw std::exception();
                }

                result = terminals[str];
                break;
        }

        return result;
    }

    Token eat(){
        Token result = token;
        next();
        return result;
    }

    bool eat(int32_t id){
        if(is(id)){
            next();
            return true;
        }
        return false;
    }

    bool is(int32_t id){
        return token.id == id;
    }

    Grammar* parse(){
        std::regex rootPattern("root", std::regex_constants::icase);
        Token parserNameToken = token;
        uint32_t ruleId = 0;
        std::vector<Rule*> rules;
        Rule* root = nullptr;
        while(!eat(EOF)){
            if(token.id != PARSER_NAME){
                throw std::exception();
            }

            Entity* owner = getEntity();

            eat(PARSER_NAME);
            eat(COLON);
            Node* node = parseOr();

            if(std::regex_match(owner->name, rootPattern)){
                if(node->rules.size() != 1){
                    throw std::exception();
                }else{
                    const std::vector<Entity*>& nodeRule = node->rules[0];
                    root = new Rule(ruleId++,owner,nodeRule);
                    rules.push_back(root);
                }
            }else{
                for( const std::vector<Entity*>& nodeRule : node->rules ){
                    Rule* rule = new Rule(ruleId++,owner,nodeRule);
                    rules.push_back(rule);
                }
            }

            delete(node);

            eat(SEMI);
        }

        if(root == nullptr){
            throw std::exception();
        }

        Entity* eof = terminals["eof"];
        std::vector<Entity*> entities;
        for(auto& pair : terminals){
            entities.push_back(pair.second);
        }

        for(auto& pair : nonTerminals){
            entities.push_back(pair.second);
        }

        auto* grammar = new Grammar(eof, root, entities, rules);
        return grammar;
    }

    Node* parseOr()
    {
        Node* left = parseConcat();

        if (eat(OR))
        {
            Node* right = parseOr();
            left->rules.insert(left->rules.begin(), right->rules.begin(), right->rules.end());
            delete(right);
        }

        return left;
    }

    Node* parseConcat()
    {
        Node* left = parseMultiplier();

        Token current = token;
        if(is(PARSER_NAME) || is(LEXER_NAME) || is(LEXER_VALUE) || is(LEFT))
        {
            Node* right = parseConcat();

            Node* result = new Node();

            for( std::vector<Entity*>& leftRule : left->rules ){
                for( std::vector<Entity*>& rightRule : right->rules ){
                    std::vector<Entity*>& unionVector = result->rules.emplace_back();
                    unionVector.insert(unionVector.end(), leftRule.begin(), leftRule.end());
                    unionVector.insert(unionVector.end(), rightRule.begin(), rightRule.end());
                }
            }

            delete(left);
            delete(right);
            return result;
        }

        return left;
    }

    Node* parseMultiplier()
    {
        Node* state = parseParenthesis();

        if (eat(STAR))
        {
        }
        else if (eat(QUESTION))
        {
            state->rules.emplace_back();
        }
        else if (eat(PLUS))
        {
        }

        return state;
    }

    Node* parseParenthesis()
    {
        if(eat(LEFT)){

            Node* state = parseOr();
            if (!eat(RIGHT))
            {
                throw std::exception();
            }

            return state;
        }


        Entity* entity = getEntity();
        if(eat(PARSER_NAME) || eat(LEXER_NAME) || eat(LEXER_VALUE))
        {
            Node* node = new Node();
            node->rules.push_back(std::vector<Entity*>({entity}));
            return node;
        }else{
            throw std::exception();
        }
    }

};
