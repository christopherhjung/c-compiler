//
// Created by Christopher Jung on 23.11.20.
//
#pragma once

#include "LexerGrammar.h"
#include "StateMachineBuilder.h"
#include "StateMachineLexer.h"
#include "../reader/FileInputReader.h"
#include "../generated/GeneratedLexer.h"
#include "LexerProxy.h"

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

int runLexer(InputReader *fileInputReader, std::ostream &out, std::ostream &err) {
    //LexerGrammar lexerGrammar = LexerGrammar::build(new FileInputReader("./resources/c.lexer"));
    //StateMachineLexer lexer(StateMachineBuilder::build(lexerGrammar));

    GeneratedLexer generatedLexer;
    LexerProxy proxy(generatedLexer);

    proxy.reset(fileInputReader);
    Token token;
    token.location.fileName = fileInputReader->getContext();
    while (proxy.hasNextToken(token)) {
        if (token.id > 0) {
            out << token.location << ": " << translate[token.id] << " " << *token.value << std::endl;
        }
    }

    out << std::flush;
    if (proxy.isError()) {
        auto error = proxy.getError();
        err << *error << std::endl;
        err << std::flush;
        delete error;
        return 1;
    }

    return 0;
}