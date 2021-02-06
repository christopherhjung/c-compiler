//
// Created by Christopher Jung on 19.11.20.
//

#include <iostream>

#include <gtest/gtest.h>
#include <gmock/gmock.h>


#include <filesystem>
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>

#include "../src/reader/FileInputReader.h"
#include "../src/lexer/lexerCore.h"
#include "../core.cpp"

#include <cstdio>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <array>

std::string filter = "";

std::string exec(std::string cmd) {
    std::array<char, 128> buffer{};
    std::string result;
    std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(cmd.c_str(), "r"), pclose);
    if (!pipe) {
        throw std::runtime_error("popen() failed!");
    }
    while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
        result += buffer.data();
    }
    return result;
}

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

int diff(const std::string& orgStr, const std::string& path, bool regex){
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
            bool match = false;
            if(regex){
                std::regex pattern(refLine);
                match = regex_match (orgLine,pattern);
            }else{
                match = refLine == orgLine;
            }

            if(!match){
                //std::cout << "\u001b[32m" << (refLine) << std::endl;
                std::cout << "\u001b[31m"  << (orgLine) << std::endl;
                error++;
            }else{
                std::cout << "\u001b[32m"  << (orgLine) << std::endl;
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
        if(!filter.empty()){
            std::regex pattern(filter);
            if(!regex_match ((std::string)dirEntry.path().filename(),pattern)){
                continue;
            }
        }

        std::cout << "------------------------------" << std::endl;
        std::cout<< "[start] " << (std::string)dirEntry.path().filename() << std::endl;

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

        currentErrors += diff(out.str(), outputRef, false);
        currentErrors += diff(errContent, errorRef, true);

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

    if(argc == 3 && std::string(argv[1]) == "-f"){
        filter = std::string(argv[2]);
    }

    errors += check("lexer", [](FileInputReader *inputReader, std::ostream &out, std::ostream &err) {
        return runLexer(inputReader, out, err);
    });

    errors += check("print_ast", [](FileInputReader *inputReader, std::ostream &out, std::ostream &err) {
        return runParser(inputReader, out, err, true, false);
    });

    errors += check("semantic_analysis", [](FileInputReader *inputReader, std::ostream &out, std::ostream &err) {
        return runParser(inputReader, out, err, true, false);
    });

    errors += check("backend", [](FileInputReader *inputReader, std::ostream &out, std::ostream &err) {
        std::string command = "../compile.sh ";
        command += inputReader->getFile();
        out << exec( command );
        return 0;
    });

    return errors;

    //testing::InitGoogleTest(&argc,argv);
    //return RUN_ALL_TESTS();
}
