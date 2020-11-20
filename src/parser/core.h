//
// Created by Christopher Jung on 19.11.20.
//

#pragma once

#include "Rule.h"
#include "Entity.h"
#include "Grammar.h"
#include "ParserGenerator.h"
#include "Parser.h"
#include "../lexer/StateMachineBuilder.h"
#include "../lexer/StateMachineLexer.h"

int runParser(InputReader* fileInputReader, std::ostream& out, std::ostream& err){



    auto* a = new Entity(0, "a", true);
    auto* b = new Entity(1, "b", true);

    auto* S = new Entity(2, "S");
    auto* A = new Entity(3, "A");
    auto* root = new Entity(4, "root");
    auto* eof = new Entity(5, "EOF", true);

    Rule* rootRule = new Rule(0, root, {S});
    Rule* ruleA = new Rule(1, S, {A,A});
    Rule* ruleB = new Rule(2, A, {a,A});
    Rule* ruleC = new Rule(3, A, {b});

    std::vector<Rule*> rules = {rootRule, ruleA, ruleB, ruleC};
    std::vector<Entity*> entities = {root, S,A,a,b, eof};

    auto* grammar = new Grammar(eof, rootRule, entities, rules);
    grammar->initialize();

    ParserGenerator generator;

    auto* table = generator.generateParser(grammar);

    Parser parser(table, grammar);

    //GeneratedLexer lexer;

    StateMachineBuilder builder;

    builder.add("a", "a", false);
    builder.add("b", "b", false);

    std::shared_ptr<State> init = builder.build();
    StateMachineLexer lexer(init);
    lexer.reset(new StringInputReader("abb"));

    auto* element = parser.parse(&lexer);

    if(element != nullptr){
        return 0;
    }else{
        return 1;
    }
}



/*
    identifier
    constant
    string-literal
    parenthesis expression,
    []
    function call
    .
    sizeof
    & unary
    * unary
    - unary
    !
    * binary
    + binary
    - binary
    <
    ==
    !=
    &&
    ||
    ?:
    =

    type-specifier:
    void
    char
    int
    struct

 */