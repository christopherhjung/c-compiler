//
// Created by Christopher Jung on 13.11.20.
//

#pragma once

#include <sstream>
#include <string>
#include <iostream>
#include <unordered_map>
#include <unordered_set>
#include <set>
#include <fstream>
#include <algorithm>

#include "StateMachineBuilder.h"

inline bool cmpState (State* l, State* r) {
    return l->index < r->index;
}

template <class T>
inline bool cmpPairState(std::pair<State*, T>& l, std::pair<State*, T>& r) {
    return cmpState(l.first, r.first);
}

class LexerGenerator {
    std::ofstream ss;
    std::string tab = "    ";
public:
    void build(const std::string& src, const std::string& target){
        std::ifstream source (src);

        std::unique_ptr<State> state;
        StateMachineBuilder builder;
        if (source.is_open())
        {
            for( std::string line; getline( source, line ); )
            {
                bool greedy = line[0] == '!';
                if(greedy){
                    line = line.substr(1);
                }

                int position = line.find(':');
                std::string key = line.substr(0, position);
                std::string value = line.substr(position + 1);

                builder.add(key, value);
            }

            state = builder.build();

            std::vector<State*> states = builder.getStates();
            std::vector<std::string> kinds = builder.getKinds();

            std::sort(states.begin(), states.end(), cmpState);

            ss.open(target);
            ss << "#include \"../lexer/Lexer.h\"" << std::endl;
            writeClass(states , state.get(), kinds);
            ss << std::flush;
            ss.close();
            source.close();
        }
    }

    void writeKinds(std::vector<std::string>& kinds){
        offset(1) << "std::string types["<<  kinds.size() <<"] {" << std::endl ;
        for( const auto& kind : kinds ){
            offset(2) << "\"" << kind << "\"," << std::endl;
        }
        offset(1) << "};" << std::endl;
    }

    void writeClass(std::vector<State*>& states, State* start, std::vector<std::string>& kinds){
        offset(0) << "class GeneratedLexer : public Lexer{" << std::endl;

        offset(1) << "char current;" << std::endl;
        offset(1) << "int32_t accept;" << std::endl;
        offset(1) << "uint32_t offset;" << std::endl;
        offset(1) << "uint32_t line = 1;" << std::endl;
        offset(1) << "uint32_t column = 1;" << std::endl;
        offset(1) << "bool error = false;" << std::endl;
        offset(1) << "Error* errorObj;" << std::endl;
        offset(1) << "char activeNewLine = 0;" << std::endl;
        offset(1) << "char lazyNewLine = 0;" << std::endl;

        writeKinds(kinds);

        offset(0) << "public:" << std::endl;

        offset(1) << "bool hasNextToken(Token& token) override {" << std::endl;

        offset(2) << "if(current == 0){" << std::endl;
        offset(3) << "return false;" << std::endl;
        offset(2) << "}" << std::endl;


        offset(2) << "accept = -1;" << std::endl;
        offset(2) << "offset = 0;" << std::endl;

        offset(2);
        callRule(start);
        ss << std::endl;

        offset(2) << "token.location.line = line;" << std::endl;
        offset(2) << "token.location.column = column;" << std::endl;

        offset(2) << "if(accept == -1){" << std::endl;

        offset(3) << "offset = reader->getOffset();" << std::endl;
        offset(3) << R"(errorObj =new Error(&token.location, reader->readString(offset ) + "_<-- char >" + current + "< wrong!" );)" << std::endl;

        offset(3) << "error = true;" << std::endl;
        offset(3) << "return false;" << std::endl;
        offset(2) << "}" << std::endl;

        offset(2) << "std::string value = reader->readString(offset);" << std::endl;
        offset(2) << "reader->setMarker(offset);" << std::endl;
        offset(2) << "current = reader->peek();" << std::endl;

        offset(2) << "for(auto& c : value){" << std::endl;
        offset(3) << "updatePosition(c);" << std::endl;
        offset(2) << "}" << std::endl;

        offset(2) << "token.id = accept;" << std::endl;
        offset(2) << "token.name = types[accept];" << std::endl;
        offset(2) << "token.value = value;" << std::endl;

        offset(2) << "return true;" << std::endl;
        offset(1) << "}" << std::endl;


        offset(1) << "void updatePosition(char c){" << std::endl;
        offset(2) << "if(activeNewLine == 0){" << std::endl;
        offset(3) << "if(c == '\\n'){" << std::endl;
        offset(4) << "activeNewLine = '\\n';" << std::endl;
        offset(4) << "lazyNewLine = '\\r';" << std::endl;
        offset(3) << "}else if(c == '\\r') {" << std::endl;
        offset(4) << "activeNewLine = '\\r';" << std::endl;
        offset(4) << "lazyNewLine = '\\n';" << std::endl;
        offset(3) << "}" << std::endl;
        offset(2) << "}" << std::endl;

        offset(2) << "if(activeNewLine == c){" << std::endl;
        offset(3) << "line++;" << std::endl;
        offset(3) << "column=1;" << std::endl;
        offset(2) << "}else if(lazyNewLine != c ){" << std::endl;
        offset(3) << "column++;" << std::endl;
        offset(2) << "}" << std::endl;
        offset(1) << "}" << std::endl;

        offset(1) << "void next() {" << std::endl;
        offset(2) << "current = reader->next();" << std::endl;
        offset(1) << "}" << std::endl;

        offset(1) << "void set(int32_t index) {" << std::endl;
        offset(2) << "accept = index;" << std::endl;
        offset(2) << "offset = reader->getOffset();" << std::endl;
        offset(1) << "}" << std::endl;

        offset(1) << "void reset(InputReader* reader) override {" << std::endl;
        offset(2) << "Lexer::reset(reader);" << std::endl;
        offset(2) << "current = reader->peek();" << std::endl;
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
        if(state->index > 0){
            offset(depth + 1) << "next();" << std::endl;
        }
        if(state->finish){
            offset(depth + 1) << "set("<< state->id <<"); //" << state->name << std::endl;
        }
        writeSwitch(state, depth + 1);
        offset(depth) << "}" << std::endl;
    }

    void writeSwitch(State* state, uint32_t depth){
        std::unordered_map<State*, std::set<char>> unsorted;
        for(const auto& pair : state->transitions){
            unsorted[pair.second].insert(pair.first);
        }

        if(unsorted.empty() && !state->greedy){
            return;
        }

        if(!unsorted.empty()) {
            offset(depth) << "switch(current){" << std::endl;
            std::vector<std::pair<State*, std::set<char>>> sorted;

            sorted.reserve(unsorted.size());
            for (auto &pair : unsorted) {
                sorted.emplace_back(pair);
            }

            std::sort(sorted.begin(), sorted.end(), cmpPairState<std::set<char>>);

            for (const auto &pair : sorted) {
                offset(depth + 1);
                uint32_t caseCount = 0;
                for (const char &c : pair.second) {
                    if (caseCount != 0 && caseCount % 8 == 0) {
                        ss << std::endl;
                        offset(depth + 1);
                    } else if (caseCount != 0) {
                        ss << " ";
                    }
                    writeCase(c);
                    caseCount++;
                }
                ss << std::endl;

                offset(depth + 2);
                callRule(pair.first);
                ss << "break;" << std::endl;
            }

            if(!state->finish){
                offset(depth + 1) << "default:" << std::endl;
                offset(depth + 2) << "accept = -1;" << std::endl;
            }

            offset(depth) << "}" << std::endl;
        }
    }

    void writeChar(char c){
        switch(c){
            case '\\': ss << "'\\\\'";
                break;
            case '\'': ss << "'\\''";
                break;
            case '\?': ss << "'\\?'";
                break;
            case '\a': ss << "'\\a'";
                break;
            case '\b': ss << "'\\b'";
                break;
            case '\f': ss << "'\\f'";
                break;
            case '\n': ss << "'\\n'";
                break;
            case '\r': ss << "'\\r'";
                break;
            case '\t': ss << "'\\t'";
                break;
            case '\v': ss << "'\\v'";
                break;
            default:
                if(c < 32){
                    ss << (int32_t)c;
                }else{
                    ss << "'"  << c << "'";
                }

        }
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
        }else if(state->greedy){
            ss << "Greedy";
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
