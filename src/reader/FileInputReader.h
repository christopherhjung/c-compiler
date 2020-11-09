//
// Created by Christopher Jung on 09.11.20.
//

#ifndef C4_FILEINPUTREADER_H
#define C4_FILEINPUTREADER_H

#include "../StreamInputReader.h"

#include <iostream>

class FileInputReader : public StreamInputReader  {

    ifstream source;
public:
    FileInputReader() {
    }


};


#endif //C4_FILEINPUTREADER_H
