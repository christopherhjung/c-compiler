
#include <iostream>

#include "reader/StreamInputReader.h"
#include "reader/FileInputReader.h"
#include "lexer/StateMachineLexer.h"

#include <functional>

#define DEBUG
//#define MEASURE
#define OUTPUT

#ifdef MEASURE
#undef OUTPUT
#endif

#ifdef DEBUG

#include "./lexer/LexerGenerator.h"
StateMachineLexer lexer("./resources/c.lexer");
#else
#include "lexer/GeneratedLexer.h"

GeneratedLexer lexer;
#endif



int main(int, char **const args) {


#ifdef DEBUG
    LexerGenerator generator;
    generator.build("./resources/c.lexer");
#endif

#ifdef MEASURE
    auto start_time = std::chrono::high_resolution_clock::now();
#endif
    InputReader* fileInputReader = new FileInputReader(args[2]);

    lexer.reset(fileInputReader);
    Token token;
    token.location.fileName = args[2];
    while(lexer.hasNextToken(token)){
        if(token.id >= 3){ //whitespace
#ifdef OUTPUT
            std::cout << token << std::endl;
#endif
        }
    }

    std::cout << std::flush;
    if(lexer.isError()){
        auto error = lexer.getError();
#ifdef OUTPUT
        std::cerr << *error << std::endl;
#endif
        delete error;
    }

#ifdef MEASURE
    auto end_time = std::chrono::high_resolution_clock::now();
    auto time = end_time - start_time;

    std::cout << "time = " << time/std::chrono::milliseconds(1)  << '\n';
#endif


    return 0;
}
