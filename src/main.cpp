
#include <iostream>
#include <functional>
#include <sstream>
#include <istream>
#include <vector>

#include "reader/StreamInputReader.h"
#include "reader/FileInputReader.h"

//#define MEASURE
#define OUTPUT

#include "generated/GeneratedLexer.h"
#include "lexer/lexerCore.h"
#include "parser/parserCore.h"

int main(int, char **const args) {


#ifdef MEASURE
    auto start_time = std::chrono::high_resolution_clock::now();
#endif

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

    //InputReader* fileInputReader = new StreamInputReader(&std::cin);

    InputReader* fileInputReader = new FileInputReader(args[2]);
    std::string command = args[1];
    std::string source = args[2];

    int code = 1;
    if(command == "--tokenize"){
        code = runLexer(fileInputReader, source, out, err);
    }else{
        code = runParser(fileInputReader, out, err);
    }


#ifdef MEASURE
    auto end_time = std::chrono::high_resolution_clock::now();
    auto time = end_time - start_time;

    msg << "time = " << time/std::chrono::milliseconds(1)  << '\n';
#endif

    return code;
}
