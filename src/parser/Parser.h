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
    Token* token = nullptr;
    std::vector<Element*> tokens;

    Element(uint32_t id) : id(id){

    }

    Element(Token* token) : id(token->id), token(token){

    }

    Element(uint32_t id, const std::vector<Element*>& tokens) : id(id), tokens(tokens){

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
        Element* currentElement = new Element(token);

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
                    stack.push_back(currentElement);

                    token = new Token();
                    if(!lexer->hasNextToken(*token)){
                        if(!lexer->isError()){
                            currentElement = new Element(grammar->end->id);
                        }else{
                            currentElement = nullptr;
                        }
                    }else{
                        currentElement = new Element(token);
                    }
                }
                else
                {
                    Rule* restoreRule = entry->restore[currentElement->id];
                    if (restoreRule != nullptr)
                    {
                        std::vector<Element*> elements(restoreRule->keys.size());

                        for (unsigned long i = restoreRule->keys.size(); i > 0; i--)
                        {
                            Element* restoreElement = stack.back();
                            stack.pop_back();

                            if (!restoreElement)
                            {
                                throw std::exception();
                            }

                            elements[i-1] = restoreElement;
                            path.pop_back();
                        }

                        currentPosition = path.back();

                        auto* element = new Element(restoreRule->owner->id, elements);
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

        uint32_t size = stack.size();

        if (size == 1)
        {
           Element* result = stack.back();

           if(result->id == grammar->root->id){
                return result;
           }
        }

        return nullptr;
    }
};
