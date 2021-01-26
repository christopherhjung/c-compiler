
#include <iostream>
#include <functional>
#include <sstream>
#include <istream>
#include <vector>

#include "src/reader/StreamInputReader.h"
#include "src/reader/FileInputReader.h"
#include "core.cpp"

//#define MEASURE
#define OUTPUT

#include "src/lexer/lexerCore.h"

#include "src/parser/SimpleParser.h"
#include "src/parser/PrettyPrinter.h"
#include "src/lexer/CatchingLexerProxy.h"
#include "src/semantic/CheckSemantic.h"
#include "src/transform/TransformContext.h"
#include "src/generated/GeneratedLexer.h"





int main(int argc, char **const args) {


#ifdef MEASURE
    auto start_time = std::chrono::high_resolution_clock::now();
#endif

#ifndef OUTPUT
    //std::ofstream ss("test/out");
    std::stringstream ss;
    std::ostream& out = ss;
    std::ostream& err = ss;
#else
    std::ostream& out = std::cout;
    std::ostream& err = std::cerr;
#endif

#ifdef MEASURE
    std::ostream& msg = std::cout;
#endif

    //InputReader* fileInputReader = new StreamInputReader(&std::cin);

    std::string command;
    std::string source;
    if(argc == 2){
        command = "--compile";
        source = args[1];
    }else if(argc == 3){
        command = args[1];
        source = args[2];
    }else{
        return 1;
    }

    bool tokenize = command == "--tokenize";
    bool printAst = command == "--print-ast";
    bool compile = command == "--compile";

    InputReader* fileInputReader = new FileInputReader(source);
    int code = 1;
    if(tokenize){
        code = runLexer(fileInputReader, out, err);
    }else{
        code = runParser(fileInputReader, out, err, printAst,compile);
    }

#ifdef MEASURE
    auto end_time = std::chrono::high_resolution_clock::now();
    auto time = end_time - start_time;

    msg << "time = " << time/std::chrono::milliseconds(1)  << '\n';
#endif

    return code;
}
