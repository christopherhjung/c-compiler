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
#include "PrettyPrinter.h"
#include "../lexer/CatchingLexerProxy.h"

int runParser(InputReader* fileInputReader, std::ostream& out, std::ostream& err, bool printAST){

    GeneratedLexer lexer;
    CatchingLexerProxy proxy(lexer);
    parser::SimpleParser parser(&proxy);
    parser.init(fileInputReader);

    try {
        auto element = parser.parse();
        if(printAST){
            PrettyPrinter printer(out);
            element->dump(printer);
            out << std::endl;
            out << std::flush;
        }
        return 0;
    }catch(parser::ParseException& e){
        std::cerr << e.error << std::endl;
        return 1;
    }
}

