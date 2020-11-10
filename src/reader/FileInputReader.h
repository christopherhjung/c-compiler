//
// Created by Christopher Jung on 09.11.20.
//

#ifndef C4_FILEINPUTREADER_H
#define C4_FILEINPUTREADER_H

#include "StreamInputReader.h"

#include <iostream>

class FileInputReader : public StreamInputReader  {

public:
    explicit FileInputReader(const std::string& file) : StreamInputReader( new std::ifstream(file)) {

    }

    virtual ~FileInputReader() {
        delete stream;
    }

    bool hasNext() override {
        if (stream->is_open())
        {
            return StreamInputReader::hasNext();
        }

        return false;
    }
};


#endif //C4_FILEINPUTREADER_H
