#include "llvm/IR/Module.h"                /* Module */
#include "llvm/IR/Function.h"              /* Function */
#include "llvm/IR/IRBuilder.h"             /* IRBuilder */
#include "llvm/IR/LLVMContext.h"           /* LLVMContext */
#include "llvm/IR/GlobalValue.h"           /* GlobaleVariable, LinkageTypes */
#include "llvm/IR/Verifier.h"              /* verifyFunction, verifyModule */
#include "llvm/Support/Signals.h"          /* Nice stacktrace output */
#include "llvm/Support/SystemUtils.h"
#include "llvm/Support/PrettyStackTrace.h"

#include "../src/transform/TransformContext.h"
#include "../src/parser/SimpleObjects.h"


const char *Filename = "fac.c";

int main(int argc, char **argv) {

    llvm::sys::PrintStackTraceOnErrorSignal(argv[0]);
    llvm::PrettyStackTraceProgram X(argc, argv);


    llvm::LLVMContext context;
    llvm::Module module("shsh", context);
    llvm::IRBuilder<> builder(context);
    llvm::IRBuilder<> allocBuilder(context);

    TransformContext transformContext(context,module, builder, allocBuilder);

    auto entry = transformContext.createFunction("main");

    auto ifStatement = new If();

    std::string name = "test";

    ifStatement->condition = new Identifier(&name);
    auto assignment = new Assignment();
    assignment->left = new Identifier(&name);
    assignment->right = new Number();
    ifStatement->trueBranch = assignment;

    try{
        ifStatement->create(transformContext, entry);
    } catch (TransformException& e) {
        std::cerr << ": error: semantic: (" << e.file << ":" << e.lineNumber << ":1) " << e.msg << std::endl;
    }

    transformContext.dump();

    return 0;
}
