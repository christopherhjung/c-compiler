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
    StreamInputReader(std::ifstream *stream){
        this->stream = stream;
    };

    virtual char peek(){
        return current;
    }

    virtual void next(){
        if(stream->eof()){
            finished = true;
        }else{
            stream->get(current);
        }
    }

    virtual bool hasNext(){
        return !finished;
    }
private:
    std::ifstream *stream;
    char current;
    bool finished = false;
};


#endif //C4_STREAMINPUTREADER_H
