//
// Created by Christopher Jung on 20.11.20.
//

#pragma once

#include "../lexer/Lexer.h"
#include "ParserTable.h"
#include "Grammar.h"
#include <vector>
#include <deque>

class Element{
public:
    uint32_t id;

    Element(uint32_t id) : id(id){

    }
};

class LeafElement : public Element{
public:
    Token* token;

    LeafElement(Token* token) : Element(token->id), token(token){

    }
};

class NodeElement : public Element{
public:
    std::vector<Element*> tokens;

    NodeElement(uint32_t id, std::vector<Element*> tokens) : Element(id), tokens(tokens){

    }
};

class Parser {
public:
    std::deque<int32_t> path;
    std::deque<Element*> stack;
    ParserTable* table;
    Grammar* grammar;

    Parser(ParserTable* table, Grammar* grammar) : table(table), grammar(grammar){

    }

    Element* parse(Lexer* lexer)
    {
        int currentPosition = 0;

        path.clear();
        stack.clear();

        path.push_back(currentPosition);

        Token* token = new Token();
        if (!lexer->hasNextToken(*token))
        {
            throw std::exception();
        }
        Element* currentElement = new LeafElement(token);

        for (; ; )
        {
            Entry* entry = table->entries[currentPosition];

            if (path.size() > stack.size())
            {
                int32_t nextPosition = entry->actions[currentElement->id];

                if (nextPosition != -1)
                {
                    currentPosition = nextPosition;
                    path.push_back(currentPosition);

                    //stack.push_back(parser::identifier("hello"));
                    stack.push_back(currentElement);

                    token = new Token();
                    if(!lexer->hasNextToken(*token)){
                        if(!lexer->isError()){
                            currentElement = new Element(grammar->end->id);
                        }else{
                            currentElement = nullptr;
                        }
                    }else{
                        currentElement = new LeafElement(token);
                    }
                }
                else
                {
                    Rule* restoreRule = entry->restore[currentElement->id];
                    std::vector<Element*> elements;
                    if (restoreRule != nullptr)
                    {
                        //Modifier modifier = source.getModifier(restoreRule);
                        for (unsigned long i = restoreRule->keys.size(); i > 0; i--)
                        {
                            //std::string key = restoreRule.getKey(i);
                            Element* restoreElement = stack.back();
                            stack.pop_back();

                            if (!restoreElement)
                            {
                                throw std::exception();
                            }

                            elements.push_back(restoreElement);

                            //modifier.register(i, restoreToken.getValue());
                            path.pop_back();
                        }

                        currentPosition = path.back();

                        auto* element = new NodeElement(restoreRule->owner->id, elements);
                        stack.push_back(element);
                    }
                    else break;
                }
            }
            else
            {
                Element* element = stack.back();
                int32_t nextPosition = entry->jumps[element->id];

                if (nextPosition == -1)
                {
                    break;
                }

                path.push_back(nextPosition);
                currentPosition = nextPosition;
            }
        }

        if (stack.size() == 1)
        {
           return stack.back();

        }

        return nullptr;
    }
};
