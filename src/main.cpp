
#include <iostream>

#include "reader/StreamInputReader.h"
#include "reader/FileInputReader.h"
#include "StateMachineBuilder.h"
#include "Lexer.h"

#include <functional>




int main(int, char **const args) {
    Lexer lexer("../resources/c.lexer");

    InputReader* fileInputReader = new FileInputReader(args[2]);
    lexer.reset(fileInputReader);
    while(lexer.hasNextToken()){
        Token token = lexer.fetchToken();
        if(token.id != 7){ //whitespace
            std::cout << token << std::endl;
        }
    }
    return 0;
}
