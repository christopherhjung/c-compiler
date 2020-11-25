//
// Created by Christopher Jung on 23.11.20.
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

int runParser(InputReader* fileInputReader, std::ostream& out, std::ostream& err){

GeneratedLexer lexer;
    parser::SimpleParser parser(&lexer);
    parser.init(fileInputReader);

    try {
        auto element = parser.parse();
        return 0;
    }catch(parser::ParseException& e){
        std::cerr << e.token << std::endl;
        return 1;
    }
}

