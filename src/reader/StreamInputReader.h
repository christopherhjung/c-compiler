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



class StreamInputReader : public InputReader{

public:
    explicit StreamInputReader(std::ifstream *stream){
        this->stream = stream;
    };

    char peek() override{
        return current;
    }

    void next() override{
        stream->get(current);
        finished = stream->eof();
    }

    bool hasNext() override{
        return !finished;
    }

protected:
    std::ifstream *stream;
    char current{};
    bool finished = false;
};


#endif //C4_STREAMINPUTREADER_H
