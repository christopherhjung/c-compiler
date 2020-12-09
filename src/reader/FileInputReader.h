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
    explicit FileInputReader(const std::string& file) : FileInputReader(file, file) {

    }

    explicit FileInputReader(const std::string& file, const std::string& context) : StreamInputReader(), fileStream(file), file(file) {
        stream = &fileStream;
        this->context = context;
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

