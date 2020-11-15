
#include <iostream>

#include "reader/StreamInputReader.h"
#include "reader/FileInputReader.h"

#include <functional>

//#define MEASURE
#define OUTPUT

#include "generated/GeneratedLexer.h"

int main(int, char **const args) {
    GeneratedLexer lexer;

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
        std::cerr << std::flush;
#endif
        delete error;
        return 1;
    }

#ifdef MEASURE
    auto end_time = std::chrono::high_resolution_clock::now();
    auto time = end_time - start_time;

    std::cout << "time = " << time/std::chrono::milliseconds(1)  << '\n';
#endif

    return 0;
}
