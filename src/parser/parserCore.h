//
// Created by Christopher Jung on 23.11.20.
//
#pragma once



#include "Rule.h"
#include "Entity.h"
#include "Grammar.h"
#include "ParserGenerator.h"
#include "../lexer/StateMachineBuilder.h"
#include "../lexer/StateMachineLexer.h"
#include "../reader/FileInputReader.h"

#include "../utils/PrintUtils.h"
#include "SimpleParser.h"
#include "PrettyPrinter.h"
#include "../lexer/CatchingLexerProxy.h"
#include "../semantic/CheckSemantic.h"

int runParser(InputReader* fileInputReader, std::ostream& out, std::ostream& err, bool printAST){

    GeneratedLexer lexer;
    CatchingLexerProxy proxy(lexer);
    SimpleParser parser(&proxy);

    try {
        parser.init(fileInputReader);
        auto* unit = parser.parse();

        try{
            Semantic checker;
            checker.check(unit);

            if(printAST){
                PrettyPrinter printer(out);
                unit->dump(printer);
                out << std::endl;
                out << std::flush;
            }
        }catch (SemanticException& e){
            err << e.location << ": error: " << std::endl;
            return 1;
        }
        return 0;
    }catch(ParseException& e){
        err << e.error << std::endl;
        return 1;
    }
}

