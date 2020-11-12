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
    int32_t marker = -1;
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
        marker = 0;
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

        tail = (tail + 1) & mask;
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
        char arr[count];
        uint32_t last = std::min(marker + count, size);

        std::copy(buffer + marker, buffer + last, arr);
        if(marker + count >= size){
            uint32_t already = last - marker;
            uint32_t rest = count - already;
            std::copy(buffer, buffer + rest, arr + already);
        }

        return std::string(arr, count);
    }

    void setMarker(uint32_t index) override {
        marker = (marker + index) & mask;
    }

    uint32_t getPosition() override {
        if(marker <= tail){
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
        int32_t start = (( head + 1 ) & mask);

        uint32_t end;
        if(marker > start){
            end = std::min((uint32_t)marker - 1, mask);
        }else{
            end = mask;
        }

        uint32_t count = end - start + 1;
        stream->read(buffer + start, count);
        count = stream->gcount();
        head = (head + count) & mask;
    }

};

