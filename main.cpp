
#include <iostream>
#include <functional>
#include <sstream>
#include <istream>
#include <vector>

#include "src/reader/StreamInputReader.h"
#include "src/reader/FileInputReader.h"

//#define MEASURE
#define OUTPUT


#include "src/lexer/lexerCore.h"

#include "src/parser/SimpleParser.h"
#include "src/parser/PrettyPrinter.h"
#include "src/lexer/CatchingLexerProxy.h"
#include "src/semantic/CheckSemantic.h"
#include "src/transform/TransformContext.h"
#include "src/generated/GeneratedLexer.h"

int runCompiler(Unit *unit, std::ostream &out, std::ostream &err, std::string filename) {
    llvm::LLVMContext context;
    llvm::Module module("shsh", context);
    llvm::IRBuilder<> builder(context);
    llvm::IRBuilder<> allocBuilder(context);

    TransformContext transformContext(context, module, builder, allocBuilder);

    try {
        unit->create(transformContext);
    } catch (TransformException &e) {
        std::cerr << ": error: semantic: (" << e.file << ":" << e.lineNumber << ":1) " << e.msg << std::endl;
    }

    transformContext.dump(filename);
    return 0;
}

int runParser(InputReader *fileInputReader, std::ostream &out, std::ostream &err, bool printAST, bool compile) {
    GeneratedLexer lexer;
    CatchingLexerProxy proxy(lexer);
    SimpleParser parser(&proxy);

    try {
        parser.init(fileInputReader);
        auto *unit = parser.parse();

        try {
            Semantic checker;
            checker.check(unit);

            if (printAST) {
                PrettyPrinter printer(out);
                unit->dump(printer);
                out << std::endl;
            }


            if(compile){
                std::string source = fileInputReader->getContext();

                std::regex filePattern(R"(([^\\/]+)\..+?$)");
                std::smatch match;
                if(std::regex_search(source,match, filePattern)){
                    std::string filename = match[1];
                    filename += ".ll";
                    return runCompiler(unit, out, err, filename);
                }
            }
        } catch (SemanticException &e) {
            err << e.location << ": error: semantic: (CheckSemantic.cpp:" << e.lineNumber << ":1) " << e.msg << std::endl;
            return 1;
        } catch (std::exception &e) {
            err << e.what() << std::endl;
            return 1;
        }
        return 0;
    } catch (ParseException &e) {
        err << e.error << " parser: " << std::endl;
        return 1;
    }
}



int main(int, char **const args) {


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

    std::string command = args[1];
    std::string source = args[2];

    bool tokenize = false;
    bool printAst = false;
    bool compile;

    std::string filename;

    if (command.rfind("--", 0) == 0) {
        tokenize = command == "--tokenize";
        printAst = command == "--print-ast";
        compile = command == "--compile";
        filename = args[2];
    }else{
        compile = true;
        filename = args[1];
    }

    InputReader* fileInputReader = new FileInputReader(filename);
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
