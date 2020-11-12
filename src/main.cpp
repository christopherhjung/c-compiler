
#include <iostream>

#include "reader/StreamInputReader.h"
#include "reader/FileInputReader.h"
#include "lexer/StateMachineBuilder.h"
#include "lexer/Lexer.h"

#include <functional>




int main(int, char **const args) {
    Lexer lexer("./resources/c.lexer");

    InputReader* fileInputReader = new FileInputReader(args[2]);

    lexer.reset(fileInputReader);
    int i = 0;
    while(lexer.hasNextToken()){
        Token token = lexer.fetchToken();
        if(token.id != 0){ //whitespace
            std::cout << i << token << std::endl;
            i++;
        }
    }

    std::cout << std::flush;
    if(lexer.isError()){
        Error error = lexer.getError();
        std::cerr << error << std::endl;
    }

    return 0;
}
