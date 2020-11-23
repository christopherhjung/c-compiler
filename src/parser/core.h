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
#include "../transformer/ParserCodeGenerator.h"
#include "../reader/FileInputReader.h"

#include "../utils/PrintUtils.h"
#include "SimpleParser.h"
#include "../parser/SimpleParser.h"

int runParser(InputReader* fileInputReader, std::ostream& out, std::ostream& err){

    /*auto* a = new Entity(0, "a", true);
    auto* b = new Entity(1, "b", true);
    auto* eof = new Entity(2, "EOF", true);

    auto* S = new Entity(0, "S");
    auto* A = new Entity(1, "A");
    auto* root = new Entity(2, "root");

    Rule* rootRule = new Rule(0, root, {S});
    Rule* ruleA = new Rule(1, S, {A,A});
    Rule* ruleB = new Rule(2, A, {a,A});
    Rule* ruleC = new Rule(3, A, {b});

    std::vector<Rule*> rules = {rootRule, ruleA, ruleB, ruleC};
    std::vector<Entity*> entities = {root, S,A,a,b, eof};

    //auto* grammar = new Grammar(eof, rootRule, entities, rules);


    //GeneratedLexer lexer;

    LexerGrammar lexerGrammar = LexerGrammar::build(new FileInputReader("./resources/c.lexer"));
    ParserCodeGenerator parserCodeGenerator(lexerGrammar);
    parserCodeGenerator.init(new FileInputReader("./resources/c.parser"));

    Grammar* grammar = parserCodeGenerator.parse();

    ParserGenerator generator;
    auto* table = generator.generateParser(grammar);

    Parser parser(table, grammar);

    StateMachineLexer lexer(StateMachineBuilder::build(lexerGrammar));
    lexer.reset(new FileInputReader("./test/test6.c"));

    Element* element = parser.parse(&lexer);*/

    parser::SimpleParser parser;
    parser.init(new FileInputReader("./test/test6.c"));

    try {
        auto element = parser.parse();
        return 0;
    }catch(std::exception e){

        return 1;
    }
}

