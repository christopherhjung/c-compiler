//
// Created by Christopher Jung on 27.11.20.
//

#pragma once

class LexerProxy;

class LexerControl {
public:
    virtual void parse() = 0;
    virtual void init(LexerProxy& proxy) = 0;
    virtual inline bool isHiding(uint32_t id) = 0;
    virtual inline bool isCatching(uint32_t id) = 0;
};
