
#include <iostream>

#include "reader/StreamInputReader.h"
#include "reader/FileInputReader.h"
#include "lexer/StateMachineBuilder.h"
#include "lexer/Lexer.h"
#include "Queue.h"

#include <functional>
#include <thread>

static Queue<Token> tokenQueue;
static Lexer lexer("./resources/c.lexer");

void search(std::string path){
    InputReader* fileInputReader = new FileInputReader(path);

    lexer.reset(fileInputReader);
    while(lexer.hasNextToken()){
        auto* token = lexer.fetchToken();
        if(token->id >= 3){ //whitespace
            tokenQueue.offer(token);
        }
    }

    tokenQueue.close();
}

#define DEBUG

int main(int, char **const args) {
#ifdef DEBUG
    auto start_time = std::chrono::high_resolution_clock::now();
#endif

    std::thread lexerThread(search,args[2]);

    for(;;){
        Token* token = tokenQueue.take();
        if(token == nullptr){
            break;
        }
        std::cout << *token << std::endl;
        delete token;
    }

    lexerThread.join();

    std::cout << std::flush;
    if(lexer.isError()){
        auto error = lexer.getError();
        std::cerr << *error << std::endl;
        delete error;
    }

#ifdef DEBUG
    auto end_time = std::chrono::high_resolution_clock::now();
    auto time = end_time - start_time;

    std::cout << "time = " << time/std::chrono::milliseconds(1)  << '\n';
#endif
    return 0;
}
