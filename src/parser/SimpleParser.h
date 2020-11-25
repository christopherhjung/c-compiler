//
// Created by Christopher Jung on 22.11.20.
//

#pragma once


#define EOF 0
#define AUTO 4
#define BREAK 5
#define CASE 6
#define CHAR 7
#define CONST 8
#define CONTINUE 9
#define DEFAULT 10
#define DO 11
#define DOUBLE 12
#define ELSE 13
#define ENUM 14
#define EXTERN 15
#define FLOAT 16
#define FOR 17
#define GOTO 18
#define IF 19
#define INLINE 20
#define INT 21
#define LONG 22
#define REGISTER 23
#define RESTRICT 24
#define RETURN 25
#define SHORT 26
#define SIGNED 27
#define SIZEOF 28
#define STATIC 29
#define STRUCT 30
#define SWITCH 31
#define TYPEDEF 32
#define UNION 33
#define UNSIGNED 34
#define VOID 35
#define VOLATILE 36
#define WHILE 37
#define ALIGNAS 38
#define ALIGNOF 39
#define ATOMIC 40
#define BOOL 41
#define COMPLEX 42
#define GENERIC 43
#define IMAGINARY 44
#define NORETURN 45
#define STATICASSERT 46
#define THREADLOCAL 47
#define DIGIT_SEQUENCE 48
#define CONSTANT 49
#define IDENTIFIER 50
#define STRING_LITERAL 51
#define LEFT_PAREN 52
#define RIGHT_PAREN 53
#define LEFT_BRACKET 54
#define RIGHT_BRACKET 55
#define LEFT_BRACE 56
#define RIGHT_BRACE 57
#define LESS 58
#define LESS_EQUAL 59
#define GREATER 60
#define GREATER_EQUAL 61
#define LEFT_SHIFT 62
#define RIGHT_SHIFT 63
#define PLUS 64
#define PLUS_PLUS 65
#define MINUS 66
#define MINUS_MINUS 67
#define STAR 68
#define DIV 69
#define MOD 70
#define AND 71
#define OR 72
#define AND_AND 73
#define OR_OR 74
#define CARET 75
#define NOT 76
#define TILDE 77
#define QUESTION 78
#define COLON 79
#define SEMI 80
#define COMMA 81
#define ASSIGN 82
#define STAR_ASSIGN 83
#define DIV_ASSIGN 84
#define MOD_ASSIGN 85
#define PLUS_ASSIGN 86
#define MINUS_ASSIGN 87
#define LEFT_SHIFT_ASSIGN 88
#define RIGHT_SHIFT_ASSIGN 89
#define AND_ASSIGN 90
#define XOR_ASSIGN 91
#define OR_ASSIGN 92
#define EQUAL 93
#define NOT_EQUAL 94
#define ARROW 95
#define DOT 96
#define ELLIPSIS 97
#define HASH 98
#define DOUBLE_HASH 99
#define LEFT_BRACKET_REPLACEMENT 100
#define RIGHT_BRACKET_REPLACEMENT 101
#define LEFT_BRACE_REPLACEMENT 102
#define RIGHT_BRACE_REPLACEMENT 103
#define HASH_REPLACEMENT 104
#define DOUBLE_HASH_REPLACEMENT 105


#include "../lexer/Lexer.h"
#include <vector>

namespace parser{
    class ParseException : public std::exception{
    public:
        Token token;

        ParseException(const Token& token) : token(token){

        }
    };

    class Element{
    public:
        virtual ~Element() = default;
    };

    class Unit : public Element{
    public:
        std::vector<Element*> children;
    };

    class Statement : public Element{

    };

    class Expression : public Statement{

    };

    class Binary : public Expression{
    public:
        Expression* left = nullptr;
        Expression* right = nullptr;
        uint32_t op;
    };

    class Unary : public Expression{
    public:
        Expression* value = nullptr;
        uint32_t op = 0;
    };

    class Return : public Statement{
    public:
        Expression* value = nullptr;
    };

    class Type : public Element{

    };


    class Identifier : public Expression{
    public:
        std::string value;
    };

    class StringLiteral : public Expression{
    public:
        std::string value;
    };

    class Constant : public Expression{
    public:
        std::string value;
    };

    class Number : public Expression{
    public:
        std::string value;
    };

    class Block : public Statement{
    public:
        std::vector<Statement*> children;
    };

    class Continue : public Statement{
    public:
    };

    class Break : public Statement{
    public:
    };

    class GoTo : public Statement{
    public:
        std::string name;
    };


    class LabeledStatement : public Statement{
    public:
        Statement* statement = nullptr;
        std::string name;
    };

    class DirectDeclarator;

    class Declarator : public Statement{
    public:
        int pointer = 0;
        DirectDeclarator* declaration = nullptr;
    };

    class DirectDeclarator : public Statement{
    public:
        Identifier* identifier;
        Declarator* declarator;
        DirectDeclarator* left;
        std::vector<Declarator*> parameter;
    };

    class Struct : public DirectDeclarator{
    public:
        Identifier* identifier = nullptr;
        std::vector<DirectDeclarator*> declarations;
    };

    class StructType : public Type{
    public:
        std::string name;
        std::vector<DirectDeclarator*> declarations;
    };

    class Method : public Element{
    public:
        Type* type;
        Identifier* name = nullptr;
        std::vector<DirectDeclarator*> params;
        Block* body;
    };

    class Call : public Expression{
    public:
        Identifier* name = nullptr;
        std::vector<Expression*> values;
    };

    class If : public Statement{
    public:
        Expression* condition = nullptr;
        Statement* trueBranch = nullptr;
        Statement* falseBranch = nullptr;
    };

    class While : public Statement{
    public:
        Expression* condition = nullptr;
        Statement* body = nullptr;
    };

    class SimpleParser {
    public:
        Lexer* lexer;
        Token* ring;


        Token lookA;
        Token lookB;
        Token lookC;

        SimpleParser(Lexer* lexer) : lexer(lexer){

        }

        void init(InputReader* parserDescriptor){
            lexer->reset(parserDescriptor);
            lookA.location.fileName = parserDescriptor->getContext();
            lookB.location.fileName = parserDescriptor->getContext();
            lookC.location.fileName = parserDescriptor->getContext();
            next();
            next();
            next();
        }

        void next(){
            lookA = lookB;
            lookB = lookC;
            lexer->hasNextToken(lookC);
        }

        Token eat(){
            Token result = lookA;
            next();
            return result;
        }

        bool eat(int32_t id){
            if(is(id)){
                next();
                return true;
            }
            return false;
        }

        void shall(int32_t id){
            if(!eat(id)){
                fatal();
            }
        }

        bool is(int32_t id){
            return lookA.id == id;
        }

        bool isB(int32_t id){
            return lookB.id == id;
        }

        void fatal(){
            throw ParseException(lookA);
        }

        Element* parse(){
            Unit* unit = new Unit();
            for(;;){
                unit->children.push_back(parseExternalDeclaration());

                if(is(EOF)){
                    break;
                }
            }

            return unit;
        }

        Element* parseExternalDeclaration(){
            Type* type = parseType();
            Declarator* declarator = parseDeclarator();
            if(is(LEFT_BRACE)){
                Block* block = parseBlock();
                Method* method = new Method();
            }else {
                shall(SEMI);
            }

            return declarator;
        }

        Method* parseFunctionDefinition(){
            auto method = new Method();
            method->type = parseType();
            method->name = parseIdentifier();
            shall(LEFT_PAREN);
            if(!is(RIGHT_PAREN)){
                method->params.push_back(parseDirectDeclarator());
                while(eat(COMMA)){
                    method->params.push_back(parseDirectDeclarator());
                }
            }
            shall(RIGHT_PAREN);
            method->body = parseBlock();
            return method;
        }

        /*
         * leftParen:\(
rightParen:\)
leftBracket:\[
rightBracket:\]
leftBrace:\{
rightBrace:\}*/

        bool isType(int32_t type){
            return type == INT || type == (CHAR) || type == (VOID) || type == (STRUCT);
        }

        Type* parseType(){
            if(is(INT) || is(CHAR) || is(VOID)){
                auto type = new Type();
                eat();
                return type;
            }else if(is(STRUCT)){
                auto structType = new StructType();
                eat();
                if(is(IDENTIFIER)){
                    structType->name = lookA.value;
                    eat();
                }

                if(is(LEFT_BRACE)){
                    parseStructDeclarationList(structType);
                }

                return structType;
            }else{
                fatal();
            }
        }

        void parseStructDeclarationList(StructType* structType){
            while(true){
                structType->declarations.push_back(parseDirectDeclarator());

                if(is(RIGHT_BRACE)){
                    break;
                }
            }
        }

        Identifier* parseIdentifier(){
            if(is(IDENTIFIER)){
                auto identifier = new Identifier();
                identifier->value = lookA.value;
                eat();
                return identifier;
            }else{
                fatal();
            }
        }

        DirectDeclarator* parseDirectDeclarator(){
            auto declaration = new DirectDeclarator();

            if(eat(LEFT_PAREN)){
                parseDeclarator();
                shall(RIGHT_PAREN);
            }else if(is(IDENTIFIER)){
                declaration->identifier = parseIdentifier();
            }else{
                throw ParseException(lookA);
            }

            while(eat(LEFT_PAREN)){
                auto inner = new DirectDeclarator();
                inner->left = declaration;
                while(true){
                    if(!isType(lookA.id)){
                        throw ParseException(lookA);
                    }
                    parseParameterTypeList(inner);

                    if(!eat(COMMA)){
                        break;
                    }
                }
                declaration = inner;
                shall(RIGHT_PAREN);
            }

            return declaration;
        }

        void parseParameterTypeList(DirectDeclarator* inner){
            Type* type = parseType();
            if(!is(COMMA) && !is(RIGHT_PAREN)){
                Declarator* declarator = parseDeclarator();
            }
        }

        Declarator* parseDeclarator(){
            auto declaration = new Declarator();

            while(eat(STAR)){
                declaration->pointer++;
            }

            declaration->declaration = parseDirectDeclarator();

            return declaration;
        }

        Struct* parseStruct(){
            shall(STRUCT);

            auto result = new Struct();
            result->identifier = parseIdentifier();
            shall(LEFT_BRACE);
            while(true){
                while(eat(SEMI));

                if(eat(RIGHT_BRACE)){
                    break;
                }

                result->declarations.push_back(parseDirectDeclarator());
            }
            shall(SEMI);
            return result;
        }

        Statement* parseStatement(){
            if(is(IF)){
                return parseIf();
            }else if(is(WHILE)){
                return parseWhile();
            }else if(is(LEFT_BRACE)) {
                return parseBlock();
            }else if(is(RETURN)) {
                return parseReturn();
            }else if(is(GOTO) ){
                return parseGoTo();
            }else if(eat(CONTINUE) ){
                shall(SEMI);
                return new Continue();
            }else if(eat(BREAK) ){
                shall(SEMI);
                return new Break();
            }else if(lookA.id == IDENTIFIER && lookB.id == COLON){
                auto result = new LabeledStatement();
                result->name = lookA.value;
                eat(IDENTIFIER);
                eat(COLON);
                result->statement = parseStatement();
                return result;
            }else{
                auto expr = parseExpression();
                shall(SEMI);
                return expr;
            }
        }

        GoTo* parseGoTo(){
            shall(GOTO);
            auto result = new GoTo();
            result->name = lookA.value;
            eat();
            shall(SEMI);
            return result;
        }

        Return* parseReturn(){
            auto result = new Return();
            eat(RETURN);
            result->value = parseExpression();
            shall(SEMI);
            return result;
        }

        Block* parseBlock(){
            auto block = new Block();
            shall(LEFT_BRACE);
            while(true){
                while(eat(SEMI));

                if(eat(RIGHT_BRACE)){
                    break;
                }

                block->children.push_back(parseBlockItem());
            }
            return block;
        }

        Statement* parseBlockItem(){
            if(isType(lookA.id)){
                return parseDirectDeclarator();
            }else{
                return parseStatement();
            }
        }

        If* parseIf(){
            auto result = new If();
            shall(IF);

            shall(LEFT_PAREN);
            result->condition = parseExpression();
            shall(RIGHT_PAREN);
            result->trueBranch = parseStatement();
            if(eat(ELSE)){
                result->falseBranch = parseBlock();
            }

            return result;
        }

        While* parseWhile(){
            auto result = new While();
            shall(WHILE);
            shall(LEFT_PAREN);
            result->condition = parseExpression();
            shall(RIGHT_PAREN);
            result->body = parseStatement();
            return result;
        }

        Expression* parseValue(){
            if(is(DIGIT_SEQUENCE)){
                auto number = new Number();
                number->value = lookA.value;
                eat();
                return number;
            }else if(is(STRING_LITERAL)){
                auto string = new StringLiteral();
                string->value = lookA.value;
                eat();
                return string;
            }else if(is(CONSTANT)){
                auto constant = new Constant();
                constant->value = lookA.value;
                eat();
                return constant;
            }else if(is(IDENTIFIER)){
                auto* identifier = parseIdentifier();
                if(eat(LEFT_PAREN)){
                    auto call = new Call();
                    call->name = identifier;
                    if(!is(RIGHT_PAREN)){
                        call->values.push_back(parseExpression());
                        while(eat(COMMA)){
                            call->values.push_back(parseExpression());
                        }
                    }
                    shall(RIGHT_PAREN);
                    return call;
                }
                return identifier;
            }else{
                fatal();
            }
        }


        Expression* parseExpression(){
            return parseExpression(20);
        }

        Expression* parseExpression(uint32_t other){
            if(eat(LEFT_PAREN)){
                Expression* inner = parseExpression();
                shall(RIGHT_PAREN);
                return inner;
            }

            Expression* left = nullptr;
            for(;;){
                uint32_t precedence = unary();
                if(precedence == 0 || left != nullptr){
                    if(left == nullptr){
                        left = parseValue();
                    }
                    precedence = binary();
                    if(precedence == 0 || precedence > other){
                        return left;
                    }else{
                        auto bin = new Binary();
                        bin->left = left;
                        auto token = eat();
                        bin->op = token.id;
                        bin->right = parseExpression(precedence);
                        left = bin;
                    }
                }else{
                    auto unary = new Unary();
                    unary->op = lookA.id;
                    eat();
                    unary->value = parseExpression(precedence);
                    left = unary;
                }
            }

            /*
             * For expressions (§6.5) we handle identifier,
             * constant, string-literal, parenthesized expression, [], function
                call, ., ->, sizeof, & (unary), * (unary),
             - (unary), !, * (binary),
             + (binary), - (binary), ,,, &&,
                ||, ?: and =. For all other expressions only
                the chain productions (A → B) are used.
             * */



            return parseIdentifier();
        }

        uint32_t binary(){
            uint32_t index = 0;
            switch(lookA.id){
                case ARROW:
                case DOT: index = 1; break;
                case PLUS:
                case MINUS: index = 4; break;
                case LESS: index = 6; break;
                case EQUAL:
                case NOT_EQUAL: index = 7; break;
                case STAR: index = 2; break;
                case AND_AND: index = 11; break;
                case OR_OR: index = 12; break;
                case ASSIGN: index = 14; break;
                default: return 0;
            }

            return index;
        }

        uint32_t unary(){
            uint32_t index = 0;
            switch(lookA.id){
                case AND:
                case PLUS:
                case MINUS:
                case STAR:
                case NOT: index = 2; break;
                default: return 0;
            }

            return index;
        }

    };
}

