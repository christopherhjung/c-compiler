//
// Created by Christopher Jung on 21.11.20.
//

#pragma once

#include <vector>
#include <unordered_map>
#include <regex>

#include "../reader/InputReader.h"

struct LexerGrammarEntry{
public:
    std::string name;
    std::string regex;
    bool greedy;
    bool hide;

    LexerGrammarEntry(const std::string& name,const std::string& regex, bool greedy, bool hide) : name(name), regex(regex), greedy(greedy), hide(hide){

    }
};

class LexerGrammar {

public:
    std::vector<LexerGrammarEntry> entries;

    LexerGrammar(){
        entries.emplace_back("eof", "", false, false);
    }

    static LexerGrammar build(InputReader* inputReader){
        LexerGrammar lexerGrammar;
        std::regex e ("\\s");

        for( std::string line; inputReader->readLine(line); )
        {
            bool hide = line[0] == '*';
            if(hide){
                line = line.substr(1);
            }

            bool greedy = line[0] == '!';
            if(greedy){
                line = line.substr(1);
            }

            int position = line.find(':');
            std::string key = line.substr(0, position);
            std::string value = line.substr(position + 1);

            key = std::regex_replace (key,e,"");
            value = std::regex_replace (value,e,"");

            if(!key.empty() && !value.empty()){
                auto& entry =lexerGrammar.entries.emplace_back(LexerGrammarEntry(key,value, greedy, hide));
            }
        }

        //delete(inputReader);
        return lexerGrammar;
    }
};
