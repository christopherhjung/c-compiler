//
// Created by Christopher Jung on 19.11.20.
//

#include <iostream>

#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "FirstTest.h"

#include <filesystem>
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>

#include "../src/reader/FileInputReader.h"
#include "../src/parser/parserCore.h"
#include "../src/lexer/lexerCore.h"

std::string removeExtension(const std::string& fileName){
    size_t position = fileName.find(".");
    return std::string::npos == position ? fileName : fileName.substr(0, position);
}

std::string findFile(const std::string& path, const std::string& name){
    if(!std::filesystem::exists(path)){
        return "";
    }

    std::string fileName = removeExtension(name);
    for (const auto& dirEntry : std::filesystem::directory_iterator(path)){
        if(removeExtension(dirEntry.path().filename()) == fileName){
            return dirEntry.path();
        }
    }

    return "";
}

int diff(const std::string& orgStr, const std::string& path){
    if(path.empty()){
        if(!orgStr.empty()){
            return 1;
        }

        return 0;
    }

    int error = 0;
    std::istringstream org(orgStr);
    std::ifstream ref(path);
    std::string orgLine;
    std::string refLine;
    for (; std::getline(org, orgLine); ) {
        if(std::getline(ref, refLine)){
            if(refLine != orgLine){
                std::cout << "\u001b[31m-" << (refLine) << std::endl;
                std::cout << "\u001b[32m+"  << (orgLine) << std::endl;
                std::cout << "\u001b[0m";
                error++;
            }
        }else{
            std::cout << "\u001b[32m+"  << orgLine << std::endl;
            std::cout << "\u001b[0m";
            error++;
        }
    }

    while(std::getline(ref, refLine)){
        std::cout << "\u001b[31m-" << refLine << std::endl;
        std::cout << "\u001b[0m";
        error++;
    }

    return error;
}

int print(const std::string& path, void (*fun)(FileInputReader*, std::ostream&, std::ostream&)){
    int errors = 0;
    for (const auto& dirEntry : std::filesystem::directory_iterator(path + "/input")){
        int currentErrors = 0;
        FileInputReader reader(dirEntry.path(), dirEntry.path().filename());

        std::stringstream out;
        std::stringstream err;
        fun(&reader, out, err);

        auto outputRef = findFile(path + "/output", reader.getContext());
        auto errorRef = findFile(path + "/error", reader.getContext());

        currentErrors += diff(out.str(), outputRef);
        currentErrors += diff(err.str(), errorRef);

        if(currentErrors == 0){
            std::cout << "\u001b[32m";
        }else{
            std::cout << "\u001b[31m";
        }

        std::cout<< path << ": " << (std::string)dirEntry.path().filename() << std::endl;
        std::cout << "\u001b[0m";

        errors += currentErrors;
    }

    return errors;
}

int main(int argc, char** argv){

    int errors = 0;

    errors += print("lexer", [](FileInputReader* inputReader, std::ostream& out, std::ostream& err){
        runLexer(inputReader, out, err);
    });

    errors += print("print_ast", [](FileInputReader* inputReader, std::ostream& out, std::ostream& err){
        runParser(inputReader, out, err, true);
    });


    return -errors;

    //testing::InitGoogleTest(&argc,argv);
    //return RUN_ALL_TESTS();
}

