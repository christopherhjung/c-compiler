//
// Created by Christopher Jung on 13.11.20.
//

#pragma once

#include <sstream>
#include <string>
#include <iostream>
#include <unordered_map>
#include <unordered_set>
#include <fstream>
#include <map>

#include "StateMachineLexer.h"

#define READER_NAME reader

bool cmpState (State* l, State* r) {
    return l->index < r->index;
}

template <class T>
bool cmpPairState(std::pair<State*, T>& l, std::pair<State*, T>& r) {
    return cmpState(l.first, r.first);
}

class LexerGenerator {
    std::ofstream ss;
    std::string tab = "    ";
public:
    void build(const std::string& file){
        std::ifstream source (file);

        std::unique_ptr<State> state;
        StateMachineBuilder builder;
        if (source.is_open())
        {
            for( std::string line; getline( source, line ); )
            {
                int position = line.find(':');
                std::string key = line.substr(0, position);
                std::string value = line.substr(position + 1);

                builder.add(key, value);
            }

            state = builder.build();

            std::vector<State*> states = builder.getStates();
            std::unordered_map<uint32_t, std::string> kinds = builder.getKinds();

            std::sort(states.begin(), states.end(), cmpState);

            ss.open("./src/lexer/GeneratedLexer.h");
            writeClass(states , state.get(), kinds);
            ss << std::flush;
            ss.close();
            source.close();
        }
    }

    void writeKinds(std::unordered_map<uint32_t, std::string>& kinds){
        offset(1) << "std::string types["<<  kinds.size() <<"] {" << std::endl ;
        for( const auto& pair : kinds ){
            offset(2) << "\"" << pair.second << "\"," << std::endl;
        }
        offset(1) << "};" << std::endl;
    }

    void writeClass(std::vector<State*>& states, State* start, std::unordered_map<uint32_t, std::string>& kinds){
        offset(0) << "class GeneratedLexer : public Lexer{" << std::endl;

        offset(1) << "char current;" << std::endl;
        offset(1) << "int32_t accept;" << std::endl;
        offset(1) << "uint32_t offset;" << std::endl;
        offset(1) << "uint32_t line = 1;" << std::endl;
        offset(1) << "uint32_t column = 1;" << std::endl;
        offset(1) << "bool error = false;" << std::endl;
        offset(1) << "Error* errorObj;" << std::endl;
        offset(1) << "Token* token;" << std::endl;

        writeKinds(kinds);

        offset(0) << "public:" << std::endl;

        offset(1) << "bool hasNextToken(Token& token) override {" << std::endl;
        offset(2) << "accept = -1;" << std::endl;
        offset(2) << "offset = 0;" << std::endl;

        offset(2);
        callRule(start);
        ss << std::endl;

        offset(2) << "token.location = new Location(reader->getOrigin(),line,column);" << std::endl;
        offset(2) << "std::string value = reader->readString(offset);" << std::endl;
        offset(2) << "reader->setMarker(offset);" << std::endl;

        offset(2) << "if(accept == -1){" << std::endl;
        offset(3) << "if(current == 0){" << std::endl;
        offset(4) << "return false;" << std::endl;
        offset(3) << "}" << std::endl;


        offset(3) << "int32_t offset = reader->getOffset() - 1;" << std::endl;
        offset(3) << "if(current == 0 && offset == 0){" << std::endl;
        offset(4) << "offset = 0;" << std::endl;
        offset(3) << "}" << std::endl;
        offset(3) << R"(errorObj =new Error(token.location, reader->readString(offset ) + "_<-- char >" + current + "< wrong!" );)" << std::endl;

        offset(3) << "error = true;" << std::endl;
        offset(3) << "return false;" << std::endl;
        offset(2) << "}" << std::endl;


        offset(2) << "for(auto& c : value){" << std::endl;
        offset(3) << "updatePosition(c);" << std::endl;
        offset(2) << "}" << std::endl;

        offset(2) << "token.id = accept;" << std::endl;
        offset(2) << "token.name = types[accept];" << std::endl;
        offset(2) << "token.value = value;" << std::endl;

        offset(2) << "return true;" << std::endl;
        offset(1) << "}" << std::endl;


        offset(1) << "void updatePosition(char c){" << std::endl;
        offset(2) << "if(c == '\\n'){" << std::endl;
        offset(3) << "line++;" << std::endl;
        offset(3) << "column=1;" << std::endl;
        offset(2) << "}else{" << std::endl;
        offset(3) << "column++;" << std::endl;
        offset(2) << "}" << std::endl;
        offset(1) << "}" << std::endl;


        offset(1) << "bool next() {" << std::endl;
        offset(2) << "reader->next();" << std::endl;
        offset(2) << "current = reader->peek();" << std::endl;
        offset(1) << "}" << std::endl;

        offset(1) << "void set(int32_t index) {" << std::endl;
        offset(2) << "accept = index;" << std::endl;
        offset(2) << "offset = reader->getOffset();" << std::endl;
        offset(1) << "}" << std::endl;

        offset(1) << "void reset(InputReader* reader) override {" << std::endl;
        offset(2) << "Lexer::reset(reader);" << std::endl;
        offset(2) << "current = reader->peek();" << std::endl;
        offset(1) << "}" << std::endl;

        offset(1) << "Token *fetchToken() override {" << std::endl;
        offset(2) << "return token;" << std::endl;
        offset(1) << "}" << std::endl;

        offset(1) << "Error *getError() override {" << std::endl;
        offset(2) << "return errorObj;" << std::endl;
        offset(1) << "}" << std::endl;

        offset(1) << "bool isError() override {" << std::endl;
        offset(2) << "return error;" << std::endl;
        offset(1) << "}" << std::endl;

        for( auto* state : states ){
            writeMethod(state, 1);
        }
        offset(0) << "};" << std::endl;
    }

    void writeMethod(State* state, uint32_t depth){
        offset(depth) << "void ";
        writeRuleName(state);
        ss << "(){" << std::endl;
        if(state->finish){
            offset(depth + 1) << "set("<< state->id <<");" << std::endl;
        }
        writeSwitch(state, depth + 1);
        offset(depth) << "}" << std::endl;
    }

    void writeSwitch(State* state, uint32_t depth){
        offset(depth) << "switch(current){" << std::endl;
        std::map<State*, std::unordered_set<char>> unsorted;
        for(const auto& pair : state->transitions){
            std::unordered_set<char>& set = unsorted[pair.second];
            unsorted[pair.second].insert(pair.first);
        }

        std::vector<std::pair<State*, std::unordered_set<char>> > sorted;

        for (auto& it : unsorted) {
            sorted.push_back(it);
        }

        std::sort(sorted.begin(), sorted.end(), cmpPairState<std::unordered_set<char>>);

        for(const auto& pair : sorted){
            offset(depth + 1);
            uint32_t caseCount = 0;
            for(const char& c : pair.second){
                if((caseCount & 7u) == 7u){
                    ss << std::endl;
                    offset(depth + 1);
                }else if(caseCount != 0){
                    ss << " ";
                }
                writeCase(c);
                caseCount++;
            }
            ss << std::endl;

            offset(depth + 2) << "next();" ;
            callRule(pair.first);
            ss << "break;" << std::endl;
        }

        offset(depth) << "}" << std::endl;
    }

    void writeEscaped(char c){

        switch(c){
            case '\'': ss << "\\\'";
                break;
            case '\"': ss << "\\\"";
                break;
            case '\?': ss << "\\?";
                break;
            case '\a': ss << "\\a;";
                break;
            case '\b': ss << "\\b";
                break;
            case '\f': ss << "\\f";
                break;
            case '\n': ss << "\\n";
                break;
            case '\r': ss << "\\r";
                break;
            case '\t': ss << "\\t";
                break;
            case '\v': ss << "\\v";
                break;
            default: ss << c;
        }
    }

    void writeChar(char c){
        ss << (int32_t)c;
    }

    void writeCase(char c){
        ss << "case ";
        writeChar(c);
        offset(0) << ":";
    }

    void callRule(State* state){
        writeRuleName(state);
        ss << "();";
    }

    void writeRuleName(State* state){
        ss << "parse" ;
        if(state->finish){
            ss << "Final";
        }
        ss << state->index;
    }

    std::ofstream& offset(uint32_t indent){
        for(;indent > 0;indent--){
            ss << tab;
        }
        return ss;
    }
};
