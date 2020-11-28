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
    std::istream *stream{};
    uint32_t tail = 0;
    uint32_t capacity = 1u << 8u;
    uint32_t size = 0;
    uint32_t mask = capacity - 1;
    uint32_t offset = 0;
    uint32_t head = 0;
    bool full = false;
    bool empty = false;
    int16_t current = 256;
    char *buffer = new char[capacity]{0};

    StreamInputReader(){

    }

    void init(){
        reset(0);
    }

public:

    std::string getContext() override {
        return "";
    }

    ~StreamInputReader() override {
        delete[](buffer);
    }
public:
    StreamInputReader(std::istream* stream) :stream(stream){
        init();
    };

    int16_t peek() override{
        return current;
    }

    int16_t next() override{
        offset++;
        if(offset < size){
            current = buffer[(tail + offset) & mask];
        }else if(empty){
            current = 256;
        }else{
            sizeUp();
            current = buffer[(tail + offset) & mask];
        }
        return current;
    }

    uint32_t getSize() override {
        return size;
    }

    int16_t get(uint32_t index) override{
        return buffer[(tail + index) & mask];
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

    bool readLine(std::string& str) override {
        if(current == 256){
            return false;
        }

        for( ; current != '\n' && current != '\r' && current != 256 ; ){
            next();
        }
        str = readString(offset );

        if(current == '\n'){
            next();
            if(current == '\r'){
                next();
            }
        }else if(current == '\r'){
            next();
            if(current == '\n'){
                next();
            }
        }

        reset(offset );
        return true;
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

    void reset(uint32_t index) override {
        tail = (tail + index) & mask;
        offset = 0;
        size -= index;
        check();
        if(size > 0){
            current = buffer[(tail + offset) & mask];
        }else{
            current = 256;
        }
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

    virtual void fetch(){
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
    }

};

