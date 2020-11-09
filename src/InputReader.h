//
// Created by Christopher Jung on 2020-11-06.
//

#ifndef C4_INPUTREADER_H
#define C4_INPUTREADER_H


#include <iostream>
#include <fstream>
#include <string>

using namespace std;

class InputReader {

public:
    InputReader(ifstream *stream){
        this->stream = stream;
    };

    char peek(){
        return current;
    }

    char next(){
        *stream >> current;
        return current;
    }

    bool hasNext(){
        return !stream->eof();
    }
private:
    ifstream *stream;
    char current;
};


#endif //C4_INPUTREADER_H
