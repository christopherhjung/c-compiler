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
    uint32_t tail = 0;
    uint32_t capacity = 1u << 8u;
    uint32_t size = 0;
    uint32_t mask = capacity - 1;
    uint32_t position = 0;
    uint32_t head = 0;
    bool full = false;
    bool empty = false;
    char *buffer = new char[capacity + 1]{0};
public:
    explicit StreamInputReader(std::ifstream *stream){
        this->stream = stream;
        fetch();
    };

    char peek() override{
        return buffer[position];
    }

    void next() override{
        if(hasCurrent()){
            position = (position + 1) & mask;
        }
    }

    bool hasCurrent() override{
        return position != head || full;
    }

    void mark() override {
        tail = position;
    }

    void reset() override {
        position = tail;
    }

    std::string readString(uint32_t count) override {
        char arr[count];
        uint32_t last = std::min(tail + count, capacity);

        std::copy(buffer + tail, buffer + last, arr);
        if(tail + count >= capacity){
            uint32_t already = last - tail;
            uint32_t rest = count - already;
            std::copy(buffer, buffer + rest, arr + already);
        }

        return std::string(arr, count);
    }

    void setMarker(uint32_t index) override {
        tail = (tail + index) & mask;
        size -= index;
        check();
    }

    uint32_t getOffset() override {
        if(tail <= position){
            return position - tail;
        }else{
            return capacity - tail + position;
        }
    }

    void check(){
        if ( size < ( capacity >> 1u ) && !empty )
        {
            fetch();
        }

        full &= tail == head;
    }

    void fetch(){
        uint32_t readCount;
        if(tail > head){
            stream->read(buffer + head, tail - head);
            readCount = stream->gcount();
        }else{
            stream->read(buffer + head, capacity - head);
            readCount = stream->gcount();
            if(readCount > 0){
                stream->read(buffer, tail);
                readCount += stream->gcount();;
            }
        }

        if(readCount == 0){
            empty = true;
        }else{
            head = (head + readCount) & mask;
            size += readCount;
            if( head == tail ){
                full = true;
            }
        }
    }

};

