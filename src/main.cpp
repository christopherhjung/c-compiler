
#include <iostream>

#include "StreamInputReader.h"
#include "RegExParser.h"
#include "Lexer.h"

#include <functional>




int main(int, char **const args) {
    Lexer lexer("../resources/c.lexer");
    std::ifstream source(args[2]);
    if (source.is_open())
    {
        InputReader* reader = new StreamInputReader(&source);
        lexer.reset(reader);
        while(lexer.hasNextToken()){
            Token token = lexer.fetchToken();
            if(token.id != 7){ //whitespace
                std::cout << token << std::endl;
            }
        }
    }
    return 0;
}
