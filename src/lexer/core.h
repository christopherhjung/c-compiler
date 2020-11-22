//
// Created by Christopher Jung on 19.11.20.
//

#pragma once

#include "LexerGrammar.h"
#include "StateMachineBuilder.h"
#include "StateMachineLexer.h"
#include "../reader/FileInputReader.h"

std::string translate[] = {
        "eof",
        "whitespace",
        "comment",
        "comment",
        "keyword",
        "keyword",
        "keyword",
        "keyword",
        "keyword",
        "keyword",
        "keyword",
        "keyword",
        "keyword",
        "keyword",
        "keyword",
        "keyword",
        "keyword",
        "keyword",
        "keyword",
        "keyword",
        "keyword",
        "keyword",
        "keyword",
        "keyword",
        "keyword",
        "keyword",
        "keyword",
        "keyword",
        "keyword",
        "keyword",
        "keyword",
        "keyword",
        "keyword",
        "keyword",
        "keyword",
        "keyword",
        "keyword",
        "keyword",
        "keyword",
        "keyword",
        "keyword",
        "keyword",
        "keyword",
        "keyword",
        "keyword",
        "keyword",
        "keyword",
        "keyword",
        "constant",
        "constant",
        "identifier",
        "string-literal",
        "punctuator",
        "punctuator",
        "punctuator",
        "punctuator",
        "punctuator",
        "punctuator",
        "punctuator",
        "punctuator",
        "punctuator",
        "punctuator",
        "punctuator",
        "punctuator",
        "punctuator",
        "punctuator",
        "punctuator",
        "punctuator",
        "punctuator",
        "punctuator",
        "punctuator",
        "punctuator",
        "punctuator",
        "punctuator",
        "punctuator",
        "punctuator",
        "punctuator",
        "punctuator",
        "punctuator",
        "punctuator",
        "punctuator",
        "punctuator",
        "punctuator",
        "punctuator",
        "punctuator",
        "punctuator",
        "punctuator",
        "punctuator",
        "punctuator",
        "punctuator",
        "punctuator",
        "punctuator",
        "punctuator",
        "punctuator",
        "punctuator",
        "punctuator",
        "punctuator",
        "punctuator",
        "punctuator",
        "punctuator",
        "punctuator",
        "punctuator",
        "punctuator",
        "punctuator",
        "punctuator",
        "punctuator"
};

int runLexer(InputReader* fileInputReader,std::string& source, std::ostream& out, std::ostream& err){
    LexerGrammar lexerGrammar = LexerGrammar::build(new FileInputReader("./resources/c.lexer"));
    StateMachineLexer lexer(StateMachineBuilder::build(lexerGrammar));

    //GeneratedLexer lexer;


    lexer.reset(fileInputReader);
    Token token;
    token.location.fileName = source;
    while(lexer.hasNextToken(token)){
        if(token.id > 0){ //eof
#ifdef OUTPUT
            out << token.location << ": " << translate[token.id] << " " << token.value << std::endl;
#endif
        }
    }

    out << std::flush;
    if(lexer.isError()){
        auto error = lexer.getError();
#ifdef OUTPUT
        err << *error << std::endl;
        err << std::flush;
#endif
        delete error;
        return 1;
    }

    return 0;
}