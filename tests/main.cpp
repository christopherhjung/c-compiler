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
    std::istringstream org(orgStr);
    std::string orgLine;
    if(path.empty()){
        if(!orgStr.empty()){
            std::cout << "\u001b[31m";
            while(std::getline(org, orgLine)){
                std::cout << orgLine << std::endl;
            }
            std::cout << "\u001b[0m";
            return 1;
        }

        return 0;
    }

    int error = 0;
    std::ifstream ref(path);
    std::string refLine;
    for (; std::getline(org, orgLine); ) {
        if(std::getline(ref, refLine)){
            if(refLine != orgLine){
                std::cout << "\u001b[34m" << (refLine) << std::endl;
                std::cout << "\u001b[31m"  << (orgLine) << std::endl;
                error++;
            }
        }else{
            std::cout << "\u001b[31m"  << orgLine << std::endl;
            error++;
        }
    }

    while(std::getline(ref, refLine)){
        std::cout << "\u001b[34m" << refLine << std::endl;
        error++;
    }
    std::cout << "\u001b[0m";

    return error;
}

int check(const std::string& path, int (*fun)(FileInputReader*, std::ostream&, std::ostream&)){
    int errors = 0;
    for (const auto& dirEntry : std::filesystem::directory_iterator(path + "/input")){
        int currentErrors = 0;
        FileInputReader reader(dirEntry.path(), dirEntry.path().filename());

        std::stringstream out;
        std::stringstream err;
        int error = fun(&reader, out, err);

        auto outputRef = findFile(path + "/output", reader.getContext());
        auto errorRef = findFile(path + "/error", reader.getContext());

        std::string errContent = err.str();

        if( (error == 0) != errContent.empty() ){
            currentErrors += 1;
        }

        currentErrors += diff(out.str(), outputRef);
        currentErrors += diff(errContent, errorRef);

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

    errors += check("lexer", [](FileInputReader *inputReader, std::ostream &out, std::ostream &err) {
        return runLexer(inputReader, out, err);
    });

    errors += check("print_ast", [](FileInputReader *inputReader, std::ostream &out, std::ostream &err) {
        return runParser(inputReader, out, err, true);
    });


    return errors;

    //testing::InitGoogleTest(&argc,argv);
    //return RUN_ALL_TESTS();
}

