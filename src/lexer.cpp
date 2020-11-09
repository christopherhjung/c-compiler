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
    ifstream myfile ("example.txt");

    cout << "start" << endl;
    if (myfile.is_open())
    {
        InputReader reader(&myfile);

        RegExParser regExParser(&myfile);

        State* state = regExParser.parse();


        for(auto key : state->transitions) {
            std::cout << key.first << endl;
        }

        cout << "finish" << endl;


        myfile.close();
    }

    else cout << "Unable to open file";

    return 0;
}

