//
// Created by Christopher Jung on 2020-11-06.
//

#pragma once



#include <iostream>
#include <fstream>
#include <string>
#include <cstdio>

#include "InputReader.h"
#include <algorithm>


class StreamInputReader : public InputReader{

protected:
    std::ifstream *stream;
    uint32_t marker = 0;
    uint32_t capacity = 8;
    uint32_t size = 1u << capacity;
    uint32_t mask = size - 1;
    uint32_t tail = 0;
    uint32_t head = mask;
    bool finished = false;
    char *buffer = new char[size + 1]{0};
public:
    explicit StreamInputReader(std::ifstream *stream){
        this->stream = stream;
        fetch();
    };

    char peek() override{
        check();
        return buffer[tail];
    }

    void next() override{
        check();
        if(head == tail){
            finished = true;
        }

        tail++;
        tail &= mask;
    }

    bool hasCurrent() override{
        return !finished;
    }

    void mark() override {
        marker = tail;
    }

    void reset() override {
        if(!finished || tail != marker){
            tail = marker;
            finished = false;
        }
    }

    std::string readString(uint32_t count) override {
        int start = tail;
        if(marker >= 0){
            start = marker;
        }

        char arr[count];
        uint32_t last = std::min(marker + count, size);

        std::copy(buffer + start, buffer + last, arr);
        if(marker + count >= size){
            uint32_t rest = marker + count - size;
            std::copy(buffer, buffer + rest, arr + rest);
        }

        return std::string(arr);
    }

    void setMarker(uint32_t index) override {
        marker = (marker + index) & mask;
    }

    uint32_t getPosition() override {
        if(marker == -1){
            return 0;
        }else if(marker <= tail){
            return tail - marker;
        }else{
            return size - marker + tail;
        }
    }

    void check(){
        if (tail == head)
        {
            fetch();
        }
    }

    void fetch(){
        uint32_t end;
        if(marker > 0){
            end = std::min(marker + 1, size);
        }else{
            end = size;
        }

        uint32_t start = (( head + 1 ) & mask);
        if(start > end){
            end = size;
        }
        uint32_t count = end - start;
        stream->read(buffer + start, count);
        count = stream->gcount();
        head = (head + count) & mask;
    }

};

