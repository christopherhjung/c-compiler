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
    std::string context;
public:
    explicit FileInputReader(const std::string& file) : StreamInputReader(), fileStream(file), file(file) {
        stream = &fileStream;
        context = file;
        init();
    }

    void fetch() override {
        if (fileStream.is_open())
        {
            StreamInputReader::fetch();
        }
    }

    std::string getContext() override {
        return context;
    }
};

