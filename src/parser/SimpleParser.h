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
        Error error;

        ParseException(const Error& error) : error(error){

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

    class Declaration : public Statement{
    public:
        Type* type;
        Declarator* declarator;
    };


    class DirectDeclarator : public Statement{
    public:
        Identifier* identifier;
        Declarator* declarator;
        DirectDeclarator* directDeclarator;
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
        std::vector<Declaration*> declarations;
    };

    class Method : public Element{
    public:
        Declaration* declaration = nullptr;
        Block* body;
    };

    class Call : public Expression{
    public:
        Expression* target = nullptr;
        std::vector<Expression*> values;
    };

    class Sizeof : public Expression{
    public:
        Type* type = nullptr;
        Declarator *declarator;
    };

    class Select : public Expression{
    public:
        Expression* target = nullptr;
        Expression* index = nullptr;
    };

    class IfSmall : public Expression{
    public:
        Expression* condition = nullptr;
        Expression* left = nullptr;
        Expression* right = nullptr;
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

        Token last;
        Token lookA;
        Token lookB;

        SimpleParser(Lexer* lexer) : lexer(lexer){

        }

        void init(InputReader* parserDescriptor){
            lexer->reset(parserDescriptor);
            lookB.location.fileName = parserDescriptor->getContext();
            lookB.end.fileName = parserDescriptor->getContext();
            next();
            next();
        }

        void next(){
            //last = lookA;
            lookA = lookB;
            lexer->hasNextToken(lookB);

            if(lexer->isError()){
                throw ParseException(*lexer->getError());
            }
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
            throw ParseException(Error(&lookA.location, "wrong token " + lookA.value));
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
            auto declaration = new Declaration();
            declaration->type = parseType();
            if(!is(SEMI)){
                declaration->declarator = parseDeclarator(true, false);
                if(is(LEFT_BRACE)){
                    auto* method = new Method();

                    method->declaration = declaration;
                    method->body = parseBlock();

                    return method;
                }
            }
            shall(SEMI);
            return declaration;
        }

        static bool isType(int32_t type){
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
                bool isIdentifier = is(IDENTIFIER);
                if(isIdentifier){
                    structType->name = lookA.value;
                    eat();
                }

                if(eat(LEFT_BRACE)){
                    parseStructDeclarationList(structType);
                    shall(RIGHT_BRACE);
                }else if(!isIdentifier){
                    fatal();
                }

                return structType;
            }else{
                fatal();
            }

            return nullptr;
        }

        void parseStructDeclarationList(StructType* structType){
            while(true){
                structType->declarations.push_back(parseDeclaration());

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

            return nullptr;
        }

        Declarator* parseDeclarator(bool normal, bool abstract){
            auto declaration = new Declarator();

            while(eat(STAR)){
                declaration->pointer++;
            }

            if(declaration->pointer > 0 && abstract && !is(LEFT_PAREN) && !(normal && is(IDENTIFIER))){
                return declaration;
            }

            declaration->declaration = parseDirectDeclarator(normal, abstract);

            return declaration;
        }

        DirectDeclarator* parseDirectDeclarator(bool normal, bool abstract){
            auto directDeclarator = new DirectDeclarator();

            if(eat(LEFT_PAREN)){
                directDeclarator->declarator = parseDeclarator(normal, abstract);
                shall(RIGHT_PAREN);
            }else if(normal && is(IDENTIFIER)){
                directDeclarator->identifier = parseIdentifier();
            }else if(!abstract){
                fatal();
            }

            while(eat(LEFT_PAREN)){
                auto inner = new DirectDeclarator();
                inner->directDeclarator = directDeclarator;
                if(!(abstract && is(RIGHT_PAREN))){
                    do {
                        parseParameterTypeList(inner);
                    } while(eat(COMMA));
                }
                directDeclarator = inner;
                shall(RIGHT_PAREN);
            }

            return directDeclarator;
        }

        void parseParameterTypeList(DirectDeclarator* inner){
            Type* type = parseType();
            if(!is(COMMA) && !is(RIGHT_PAREN)){
                Declarator* declarator = parseDeclarator(true, true);
                inner->parameter.push_back(declarator);
            }
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
            if(!is(SEMI)){
                result->value = parseExpression();
            }
            shall(SEMI);
            return result;
        }

        Block* parseBlock(){
            auto block = new Block();
            shall(LEFT_BRACE);
            for(;;){
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
                return parseDeclaration();
            }else{
                return parseStatement();
            }
        }

        Declaration* parseDeclaration(){
            auto declaration = new Declaration();
            declaration->type = parseType();
            if(!is(SEMI)){
                declaration->declarator = parseDeclarator(true, false);
            }
            shall(SEMI);
            return declaration;
        }

        If* parseIf(){
            auto result = new If();
            shall(IF);
            shall(LEFT_PAREN);
            result->condition = parseExpression();
            shall(RIGHT_PAREN);
            result->trueBranch = parseStatement();
            if(eat(ELSE)){
                result->falseBranch = parseStatement();
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
                return parseIdentifier();
            }

            fatal();
        }


        Expression* parseExpression(){
            return parseExpression(20);
        }

        Expression* parseExpression(uint32_t other){

            Expression* left = nullptr;
            uint32_t precedence = unary();

            if(precedence != 0){
                uint32_t op = lookA.id;
                eat();

                if(op == SIZEOF && isType(lookB.id)) {
                    shall(LEFT_PAREN);
                    auto sizeofObj = new Sizeof();
                    sizeofObj->type = parseType();
                    if(!is(RIGHT_PAREN)){
                        sizeofObj->declarator = parseDeclarator(false, true);
                    }
                    shall(RIGHT_PAREN);
                    left = sizeofObj;
                }else{
                    auto unary = new Unary();
                    unary->op = op;
                    if(unary->op == LEFT_PAREN){
                        left = parseExpression();
                        shall(RIGHT_PAREN);
                    }else{
                        unary->value = parseExpression(precedence);
                        left = unary;
                    }
                }
            }else{
                left = parseValue();
            }

            for(;;){
                precedence = binary();
                if(precedence == 0 || precedence > other){
                    return left;
                }else{
                    if(eat(QUESTION)){
                        auto ifSimple = new IfSmall();
                        ifSimple->condition = left;
                        ifSimple->left = parseExpression();
                        shall(COLON);
                        ifSimple->right = parseExpression();
                        left = ifSimple;
                    }else if(eat(LEFT_PAREN)){
                        auto call = new Call();
                        call->target = left;
                        if(!is(RIGHT_PAREN)){
                            do {
                                call->values.push_back(parseExpression());
                            } while(eat(COMMA));
                        }
                        shall(RIGHT_PAREN);
                        left = call;
                    }else if(eat(LEFT_BRACKET)){
                        auto select = new Select();
                        select->target = left;
                        select->index = parseExpression();
                        shall(RIGHT_BRACKET);
                        left = select;
                    }else {
                        auto bin = new Binary();
                        bin->left = left;
                        auto token = eat();
                        bin->op = token.id;
                        if (token.id == ARROW || token.id == DOT) {
                            bin->right = parseIdentifier();
                        } else {
                            bin->right = parseExpression(precedence);
                        }
                        left = bin;
                    }
                }
            }
        }

        uint32_t binary(){
            uint32_t index = 0;
            switch(lookA.id){
                case ARROW:
                case LEFT_PAREN:
                case LEFT_BRACKET:
                case DOT: index = 1; break;
                case STAR: index = 2; break;
                case PLUS:
                case MINUS: index = 4; break;
                case LESS: index = 6; break;
                case EQUAL:
                case NOT_EQUAL: index = 7; break;
                case AND_AND: index = 11; break;
                case OR_OR: index = 12; break;
                case QUESTION: index = 13; break;
                case ASSIGN: index = 14; break;
                default: return 0;
            }

            return index;
        }

        uint32_t unary(){
            uint32_t index = 0;
            switch(lookA.id){
                case LEFT_PAREN: index = 1; break;
                case SIZEOF:
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

