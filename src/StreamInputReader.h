//
// Created by Christopher Jung on 2020-11-06.
//

#ifndef C4_STREAMINPUTREADER_H
#define C4_STREAMINPUTREADER_H


#include <iostream>
#include <fstream>
#include <string>
#include <cstdio>

#include "InputReader.h"

using namespace std;

class StreamInputReader : public InputReader{

public:
    StreamInputReader(ifstream *stream){
        this->stream = stream;
    };

    virtual char peek(){
        return current;
    }

    virtual char next(){
        stream->get(current);
        return current;
    }

    virtual bool hasNext(){
        return !stream->eof();
    }
private:
    ifstream *stream;
    char current;
};


#endif //C4_STREAMINPUTREADER_H
