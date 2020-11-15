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
    uint32_t offset = 0;
    uint32_t head = 0;
    bool full = false;
    bool empty = false;
    char current = 0;
    char *buffer = new char[capacity]{0};
public:
    explicit StreamInputReader(std::ifstream *stream){
        this->stream = stream;
        fetch();
    };

    char peek() override{
        return current;
    }

    char next() override{
        offset++;
        if(offset < size){
            current = buffer[(tail + offset) & mask];
        }else if(empty){
            current = 0;
        }else{
            sizeUp();
            current = buffer[(tail + offset) & mask];
        }
        return current;
    }

    void reset() override {
        current = buffer[(tail + offset) & mask];
    }

    uint32_t getSize() override {
        return size;
    }

    std::string readString(uint32_t count) override {
        char arr[count];
        uint32_t last = tail + count;

        if(last > capacity){
            last = capacity;
        }

        std::copy(buffer + tail, buffer + last, arr);
        if(tail + count >= capacity){
            uint32_t already = last - tail;
            uint32_t rest = count - already;
            std::copy(buffer, buffer + rest, arr + already);
        }

        return std::string(arr, count);
    }

    void sizeUp()  {
        uint32_t nextCapacity = capacity << 1u;

        char* arr = new char[nextCapacity];

        if(head > tail){
            std::copy(buffer + tail, buffer + head, arr);
        }else{
            std::copy(buffer + tail, buffer + capacity, arr);
            std::copy(buffer, buffer + head, arr + capacity - tail);
        }

        capacity = nextCapacity;
        mask = capacity - 1;
        tail = 0;
        head = size;

        delete[](buffer);
        buffer = arr;

        fetch();
    }

    void setMarker(uint32_t index) override {
        tail = (tail + index) & mask;
        offset = 0;
        size -= index;
        check();
    }

    uint32_t getOffset() override {
        return offset;
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

        head = (head + readCount) & mask;
        size += readCount;
        if( head == tail ){
            full = true;
        }else{
            empty = true;
        }
        //current = buffer[position];
    }

};

