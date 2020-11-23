#include "lexer/LexerGenerator.h"

int main(int, char ** const args) {
    LexerGenerator generator;

    generator.build(args[1], args[2]);
    return 0;
}
