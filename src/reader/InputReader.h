//
// Created by Christopher Jung on 09.11.20.
//

#ifndef C4_INPUTREADER_H
#define C4_INPUTREADER_H


class InputReader {
public:
    virtual char peek() = 0;
    virtual void next() = 0;
    virtual bool hasNext() = 0;
};


#endif //C4_INPUTREADER_H
