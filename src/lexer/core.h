//
// Created by Christopher Jung on 19.11.20.
//

#pragma once

std::string* array;

void readList(){
    std::ifstream source ("./resources/c.translate");

    std::vector<std::string> list;
    if (source.is_open())
    {
        for( std::string line; getline( source, line ); )
        {
            list.push_back(line);
        }
    }

    array =  new std::string[list.size()];
    std::copy(list.begin(), list.end(), array);
}

int runLexer(InputReader* fileInputReader,std::string& source, std::ostream& out, std::ostream& err){
    GeneratedLexer lexer;
    readList();

    lexer.reset(fileInputReader);
    Token token;
    token.location.fileName = source;
    while(lexer.hasNextToken(token)){
        if(token.id >= 3){ //whitespace
#ifdef OUTPUT
            out << token.location << ": " << array[token.id] << " " << token.value << std::endl;
#endif
        }
    }

    out << std::flush;
    if(lexer.isError()){
        auto error = lexer.getError();
#ifdef OUTPUT
        err << *error << std::endl;
        err << std::flush;
#endif
        delete error;
        return 1;
    }

    return 0;
}