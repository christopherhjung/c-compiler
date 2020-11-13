
#include <iostream>

#include "reader/StreamInputReader.h"
#include "reader/FileInputReader.h"
#include "lexer/StateMachineBuilder.h"
#include "lexer/Lexer.h"

#include <functional>


int main(int, char **const args) {
    auto start_time = std::chrono::high_resolution_clock::now();

    Lexer lexer("./resources/c.lexer");

    InputReader* fileInputReader = new FileInputReader(args[2]);

    lexer.reset(fileInputReader);
    while(lexer.hasNextToken()){
        auto token = lexer.fetchToken();
        if(token->id >= 3){ //whitespace
            std::cout << *token << std::endl;
        }
        delete token;
    }

    std::cout << std::flush;
    if(lexer.isError()){
        auto error = lexer.getError();
        std::cerr << *error << std::endl;
        delete error;
    }

    auto end_time = std::chrono::high_resolution_clock::now();
    auto time = end_time - start_time;

    std::cout << "time = " << time/std::chrono::milliseconds(1)  << '\n';
    return 0;
}
