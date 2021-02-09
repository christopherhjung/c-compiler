//
// Created by chris on 24.01.21.
//

#include <iostream>
#include <functional>
#include <sstream>
#include <istream>
#include <vector>

#include "src/reader/StreamInputReader.h"
#include "src/reader/FileInputReader.h"
#include "src/elements/Unit.h"

#include <regex>

#include "llvm/Support/FileSystem.h"
#include "llvm/Support/raw_ostream.h"

#include "llvm/IR/Module.h"                /* Module */
#include "llvm/IR/Function.h"              /* Function */
#include "llvm/IR/IRBuilder.h"             /* IRBuilder */
#include "llvm/IR/LLVMContext.h"           /* LLVMContext */
#include "llvm/IR/GlobalValue.h"           /* GlobaleVariable, LinkageTypes */
#include "llvm/IR/Verifier.h"              /* verifyFunction, verifyModule */
#include "llvm/Support/Signals.h"          /* Nice stacktrace output */
#include "llvm/Support/SystemUtils.h"
#include "llvm/Support/PrettyStackTrace.h"
#include "src/transform/TransformContext.h"
#include "src/generated/GeneratedLexer.h"
#include "src/parser/SimpleParser.h"
#include "src/semantic/CheckSemantic.h"
#include "src/parser/PrettyPrinter.h"
#include "src/elements/Elements.h"


int runCompiler(Unit *unit, std::ostream &out, std::ostream &err, const std::string &sourceFile, const std::string &targetFile) {
    llvm::LLVMContext context;
    llvm::Module module(sourceFile, context);
    llvm::IRBuilder<> builder(context);
    llvm::IRBuilder<> allocBuilder(context);

    TransformContext transformContext(context, module, builder, allocBuilder);

    try {
        unit->create(transformContext);
        transformContext.dump(targetFile);
    } catch (TransformException &e) {
        std::cerr << ": error: semantic: (" << e.file << ":" << e.lineNumber << ":1) " << e.msg << std::endl;
    }

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
                    std::string target = match[1];
                    target += ".ll";
                    return runCompiler(unit, out, err, source, target);
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