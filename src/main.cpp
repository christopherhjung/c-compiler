
#include <iostream>
#include <functional>
#include <sstream>
#include <istream>

#include "reader/StreamInputReader.h"
#include "reader/FileInputReader.h"

//#define MEASURE
#define OUTPUT

#include "generated/GeneratedLexer.h"

int main(int, char **const args) {
    GeneratedLexer lexer;

#ifdef MEASURE
    auto start_time = std::chrono::high_resolution_clock::now();
#endif
    //InputReader* fileInputReader = new StreamInputReader(&std::cin);

    InputReader* fileInputReader = new FileInputReader(args[2]);
#ifdef MEASURE
    //std::ofstream ss("test/out");
    std::stringstream ss;
    std::ostream& out = ss;
    std::ostream& err = ss;
    std::ostream& msg = std::cout;
#else
    std::ostream& out = std::cout;
    std::ostream& err = std::cerr;
#endif

    lexer.reset(fileInputReader);
    Token token;
    token.location.fileName = args[2];
    while(lexer.hasNextToken(token)){
        if(token.id >= 3){ //whitespace
#ifdef OUTPUT
            out << token << std::endl;
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

#ifdef MEASURE
    auto end_time = std::chrono::high_resolution_clock::now();
    auto time = end_time - start_time;

    msg << "time = " << time/std::chrono::milliseconds(1)  << '\n';
#endif

    return 0;
}
