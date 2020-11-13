
#include <iostream>

#include "reader/StreamInputReader.h"
#include "reader/FileInputReader.h"
#include "lexer/StateMachineBuilder.h"
#include "lexer/Lexer.h"

#include <functional>


int main(int, char **const args) {
    Lexer lexer("./resources/c.lexer");

    InputReader* fileInputReader = new FileInputReader(args[2]);

    int i = 0;
    lexer.reset(fileInputReader);
    while(lexer.hasNextToken()){
        auto token = lexer.fetchToken();
        if(token->id >= 1){ //whitespace
            std::cout << *token << std::endl;
            std::cout << std::flush;
        }
    }

    std::cout << std::flush;
    if(lexer.isError()){
        auto error = lexer.getError();
        std::cerr << *error << std::endl;
    }

    return 0;
}
