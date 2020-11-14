//
// Created by Christopher Jung on 09.11.20.
//

#pragma once


#include "StreamInputReader.h"

#include <iostream>
#include <string>

class FileInputReader : public StreamInputReader  {

    std::string file;
public:
    explicit FileInputReader(const std::string& file) : StreamInputReader( new std::ifstream(file)), file(file) {
    }

    virtual ~FileInputReader() {
        delete stream;
    }

    bool hasCurrent() override {
        if (stream->is_open())
        {
            return StreamInputReader::hasCurrent();
        }

        return false;
    }

    std::string getOrigin() override {
        return file;
    }
};

