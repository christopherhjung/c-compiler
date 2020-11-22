//
// Created by Christopher Jung on 09.11.20.
//

#pragma once


#include "StreamInputReader.h"

#include <iostream>
#include <string>

class FileInputReader : public StreamInputReader  {
    std::ifstream fileStream;
    std::string file;
public:
    explicit FileInputReader(const std::string& file) : StreamInputReader(), fileStream(file), file(file) {
        stream = &fileStream;
        init();
    }

    void fetch() override {
        if (fileStream.is_open())
        {
            StreamInputReader::fetch();
        }
    }
};

