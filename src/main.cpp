
#include <iostream>

#include "StreamInputReader.h"
#include "RegExParser.h"
#include "Lexer.h"

#include <functional>


using namespace std;






int main(int, char **const) {
    Lexer lexer("../resources/c.lexer");
    ifstream source("../resources/test.c");
    if (source.is_open())
    {
        InputReader* reader = new StreamInputReader(&source);
        lexer.reset(reader);
        while(lexer.hasNextToken()){
            cout << lexer.fetchToken() << endl;
        }
    }
    return 0;
}
