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
#include "../reader/FileInputReader.h"

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
        FileInputReader inputReader(src);

        std::shared_ptr<State> state;
        StateMachine* machine = StateMachineBuilder::build(&inputReader);

        StateMachineBuilder builder;
        std::vector<State*> states = builder.getStates();
        std::vector<std::string> kinds = builder.getKinds();

        ss.open(target);
        ss << "#pragma once" << std::endl;
        ss << "#include \"../lexer/Lexer.h\"" << std::endl;
        writeClass(machine->states , machine->root, machine->kinds, machine->hides);
        ss << std::flush;
        ss.close();
    }

    void writeKinds(std::vector<std::string>& kinds){
        offset(1) << "std::string types["<<  kinds.size() <<"] {" << std::endl ;
        for( const auto& kind : kinds ){
            offset(2) << "\"" << kind << "\"," << std::endl;
        }
        offset(1) << "};" << std::endl;
    }

    void writeHides(std::vector<bool>& hides){
        offset(1) << "bool hides["<<  hides.size() <<"] {" << std::endl ;
        for( const auto& hide : hides ){
            offset(2) << !hide << "," << std::endl;
        }
        offset(1) << "};" << std::endl;
    }

    void writeClass(std::vector<State*>& states, State* start, std::vector<std::string>& kinds, std::vector<bool>& hides){
        std::sort(states.begin(), states.end(), cmpState);

        offset(0) << "class GeneratedLexer : public Lexer{" << std::endl;

        offset(1) << "int16_t current;" << std::endl;
        offset(1) << "int32_t accept;" << std::endl;
        offset(1) << "uint32_t offset;" << std::endl;
        offset(1) << "uint32_t line = 1;" << std::endl;
        offset(1) << "uint32_t column = 1;" << std::endl;
        offset(1) << "bool error = false;" << std::endl;
        offset(1) << "Error* errorObj;" << std::endl;
        offset(1) << "char last = 0;" << std::endl;
        offset(1) << "bool finish = false;" << std::endl;

        writeKinds(kinds);
        writeHides(hides);

        offset(0) << "public:" << std::endl;

        offset(1) << "bool hasNextToken(Token& token) override {" << std::endl;


        offset(2) << "std::string value;" << std::endl;

        offset(2) << "while(true){" << std::endl;

        offset(3) << "if(current == 256){" << std::endl;
        offset(4) << "if(finish){" << std::endl;
        offset(5) << "return false;" << std::endl;
        offset(4) << "}else{" << std::endl;
        offset(5) << "token.id = 0;" << std::endl;
        offset(5) << "token.value = \"\";" << std::endl;
        offset(5) << "finish = true;" << std::endl;
        offset(5) << "return true;" << std::endl;
        offset(4) << "}" << std::endl;
        offset(3) << "}" << std::endl;

        offset(3) << "accept = -1;" << std::endl;
        offset(3) << "offset = 0;" << std::endl;

        offset(3);
        callRule(start);
        ss << std::endl;

        offset(3) << "token.location.line = line;" << std::endl;
        offset(3) << "token.location.column = column;" << std::endl;

        offset(3) << "if(accept == -1){" << std::endl;

        offset(4) << "offset = reader->getOffset();" << std::endl;

        offset(4) << "if(current == 256){" << std::endl;
        offset(5) << R"(errorObj =new Error(&token.location, reader->readString(offset ) + "_<-- char >EOL< wrong!" );)" << std::endl;
        offset(4) << "}else if(current == 0){" << std::endl;
        offset(5) << R"(errorObj =new Error(&token.location, reader->readString(offset ) + "_<-- char >NULL< wrong!" );)" << std::endl;
        offset(4) << "}else{" << std::endl;
        offset(5) << R"(errorObj =new Error(&token.location, reader->readString(offset ) + "_<-- char >" + ((char)current) + "< wrong!" );)" << std::endl;
        offset(4) << "}" << std::endl;

        offset(4) << "error = true;" << std::endl;
        offset(4) << "return false;" << std::endl;
        offset(3) << "}" << std::endl;

        offset(3) << "value = reader->readString(offset);" << std::endl;
        offset(3) << "reader->reset(offset);" << std::endl;
        offset(3) << "current = reader->peek();" << std::endl;

        offset(3) << "for(auto& c : value){" << std::endl;
        offset(4) << "updatePosition(c);" << std::endl;
        offset(3) << "}" << std::endl;


        offset(3) << "if(hides[accept]){" << std::endl;
        offset(4) << "break;" << std::endl;
        offset(3) << "}" << std::endl;

        offset(2) << "}" << std::endl;





        offset(2) << "token.id = accept;" << std::endl;
        //offset(2) << "token.name = types[accept];" << std::endl;
        offset(2) << "token.value = value;" << std::endl;







        offset(2) << "return true;" << std::endl;
        offset(1) << "}" << std::endl;

        offset(1) << "void updatePosition(char c){" << std::endl;
        offset(2) << "if(c == '\\r'){" << std::endl;
        offset(3) << "if(last == '\\n'){" << std::endl;
        offset(4) << "last = 0;" << std::endl;
        offset(3) << "}else{" << std::endl;
        offset(4) << "column=1;" << std::endl;
        offset(4) << "line++;" << std::endl;
        offset(4) << "last = c;" << std::endl;
        offset(3) << "}" << std::endl;
        offset(2) << "}else if(c == '\\n'){" << std::endl;
        offset(3) << "if(last == '\\r'){" << std::endl;
        offset(4) << "last = 0;" << std::endl;
        offset(3) << "}else{" << std::endl;
        offset(4) << "column=1;" << std::endl;
        offset(4) << "line++;" << std::endl;
        offset(4) << "last = c;" << std::endl;
        offset(3) << "}" << std::endl;
        offset(2) << "}else{" << std::endl;
        offset(3) << "column++;" << std::endl;
        offset(3) << "last = c;" << std::endl;
        offset(2) << "}" << std::endl;
        offset(1) << "}" << std::endl;

        offset(1) << "void next() {" << std::endl;
        offset(2) << "current = reader->next();" << std::endl;
        offset(1) << "}" << std::endl;

        offset(1) << "void set(int32_t id) {" << std::endl;
        offset(2) << "accept = id;" << std::endl;
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

            if(state->greedy){
                offset(depth + 1) << "default:" << std::endl;
                offset(depth + 2) << "accept = -1;" << std::endl;
            }

            offset(depth) << "}" << std::endl;
        }
    }

    uint32_t writeChar(char c){
        switch(c){
            case '\\':
                ss << "'\\\\'";
                return 4;
            case '\'':
                ss << "'\\''";
                return 4;
            case '\?':
                ss << "'\\?'";
                return 4;
            case '\a':
                ss << "'\\a'";
                return 4;
            case '\b':
                ss << "'\\b'";
                return 4;
            case '\f':
                ss << "'\\f'";
                return 4;
            case '\n':
                ss << "'\\n'";
                return 4;
            case '\r':
                ss << "'\\r'";
                return 4;
            case '\t':
                ss << "'\\t'";
                return 4;
            case '\v':
                ss << "'\\v'";
                return 4;
            default:
                if(c < 32 || c == 127){
                    ss << (int32_t)c;
                    if(c == 127){
                        return 3;
                    }else if(c > 9) {
                        return 2;
                    }else if(c >= 0){
                        return 1;
                    }else  if(c < -99){
                        return 4;
                    }else if(c < -9){
                        return 3;
                    }else{
                        return 2;
                    }
                }else{
                    ss << "'"  << c << "'";
                    return 3;
                }
        }
    }

    void writeCase(char c){
        ss << "case ";
        uint32_t count = writeChar(c);
        for(;count < 4;count++){
            ss << " ";
        }
        ss << ":";
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
