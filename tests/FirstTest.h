//
// Created by Christopher Jung on 15.11.20.
//

#pragma once

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "../src/generated/GeneratedLexer.h"
#include "../src/reader/StringInputReader.h"

using testing::Eq;

namespace{
    class Body : testing::Test{
    public:
        GeneratedLexer lexer;
        StringInputReader reader;
        Body(std::string str){
            reader.reset(str);
            lexer.reset(&reader);
        }

        void tokenEquals(int line, int column, std::string type, std::string value){
            Token token;
            bool found = false;
            while(lexer.hasNextToken(token)){
                if(token.id >= 3){
                    found = true;
                    break;
                }
            }

            if(token.id < 3){
                EXPECT_TRUE(false);
                return;
            }

            EXPECT_TRUE(found);
            EXPECT_EQ(token.location.line, line);
            EXPECT_EQ(token.location.column, column);
            EXPECT_EQ(token.name, type);
            EXPECT_EQ(token.value, value);
            EXPECT_FALSE(lexer.isError());
        }

        void noToken(){
            Token token;
            EXPECT_FALSE(lexer.hasNextToken(token));
        }

        void hasError(){
            Token token;
            EXPECT_TRUE(lexer.isError());
        }

        void hasError(std::string msg){
            Token token;
            hasError();
            Error* error = lexer.getError();
            EXPECT_EQ(error->msg, msg);
        }

    private:
        void TestBody() override {

        }
    };
}

TEST(Body, Comment_Test) {
    Body body("/*");
    body.noToken();
    body.hasError("/*_<-- char >EOL< wrong!");
}

TEST(Comment_Test2, TestEq) {
    Body body("/**");
    body.noToken();
    body.hasError("/**_<-- char >EOL< wrong!");
}

TEST(Sheet_Test, Sheet1A) {
    Body body("42  if\r\n\t\"bla\\n\"x+");
    body.tokenEquals(1,1,"constant", "42");
    body.tokenEquals(1,5,"keyword", "if");
    body.tokenEquals(2,2,"string-literal", "\"bla\\n\"");
    body.tokenEquals(2,9,"identifier", "x");
    body.tokenEquals(2,10,"punctuator", "+");
}

TEST(Sheet_Test, Sheet1B) {
    Body body("42  if\r\t\"bla\\n\"x+");
    body.tokenEquals(1,1,"constant", "42");
    body.tokenEquals(1,5,"keyword", "if");
    body.tokenEquals(2,2,"string-literal", "\"bla\\n\"");
    body.tokenEquals(2,9,"identifier", "x");
    body.tokenEquals(2,10,"punctuator", "+");
}

TEST(Sheet_Test, Sheet1C) {
    Body body("42  if\n\r\t\"bla\\n\"x+");
    body.tokenEquals(1,1,"constant", "42");
    body.tokenEquals(1,5,"keyword", "if");
    body.tokenEquals(2,2,"string-literal", R"("bla\n")");
    body.tokenEquals(2,9,"identifier", "x");
    body.tokenEquals(2,10,"punctuator", "+");
}

TEST(Sheet_Test, Sheet1D) {
    Body body("42  if\n\t\"bla\\n\"x+");
    body.tokenEquals(1,1,"constant", "42");
    body.tokenEquals(1,5,"keyword", "if");
    body.tokenEquals(2,2,"string-literal", "\"bla\\n\"");
    body.tokenEquals(2,9,"identifier", "x");
    body.tokenEquals(2,10,"punctuator", "+");
}

TEST(Sheet_Test, Sheet1E) {
    Body body("42  if\n \"bla\\n\"x+");
    body.tokenEquals(1,1,"constant", "42");
    body.tokenEquals(1,5,"keyword", "if");
    body.tokenEquals(2,2,"string-literal", "\"bla\\n\"");
    body.tokenEquals(2,9,"identifier", "x");
    body.tokenEquals(2,10,"punctuator", "+");
}

TEST(Sheet_Test, CommentTestA) {
    Body body("int test;\r\n/*Com\nment*/a");
    body.tokenEquals(1,1,"keyword", "int");
    body.tokenEquals(1,5,"identifier", "test");
    body.tokenEquals(1,9,"punctuator", ";");
    body.tokenEquals(3,7,"identifier", "a");
}