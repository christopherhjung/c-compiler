//
// Created by Christopher Jung on 2020-11-06.
//


#include <iostream>
#include <fstream>
#include <string>

#include "InputReader.h"
#include "RegExParser.h"

#include <unordered_set>
#include <unordered_map>
#include <functional>

using namespace std;


int runTest(){
    string line;
    ifstream myfile ("../example.txt");

    cout << "start" << endl;
    if (myfile.is_open())
    {
        RegExParser regExParser(&myfile);

        regExParser.add("first", "a(ab*)*");

        State* state = regExParser.build();
        while(true){
            for(auto key : state->transitions) {
                std::cout << key.first << ", ";
            }

            if(state->finish){
                cout << "!!!";
            }
            std::cout << endl;

            char c;
            std::cin >> c;

            if(c == 1){
                break;
            }

            state = state->transitions[c];
        }

        cout << "finish" << endl;


        myfile.close();
    }

    else cout << "Unable to open file";

    return 0;
}

