//
// Created by Christopher Jung on 22.11.20.
//

#pragma once


#include "TypeDefines.h"
#include "../lexer/Lexer.h"
#include "SimpleObjects.h"
#include <vector>


namespace parser{
    class ParseException : public std::exception{
    public:
        Error error;

        explicit ParseException(const Error& error) : error(error){

        }
    };

    class Descriptor{
    public:
        std::vector<Type*> types;
        bool filled = false;
        bool defined = false;
    };


    class SimpleParser {
    public:
        Lexer* lexer;

        Token lookA;
        Token lookB;
        bool insideLoop = false;

        std::unordered_map<const std::string*, Descriptor> defs;
        std::unordered_map<const DirectDeclarator*, Descriptor> defs2;

        explicit SimpleParser(Lexer* lexer) : lexer(lexer){

        }

        void init(InputReader* parserDescriptor){
            lexer->reset(parserDescriptor);
            lookB.location.fileName = parserDescriptor->getContext();
            next();
            next();
        }

        void next(){
            lookA = lookB;
            lexer->hasNextToken(lookB);

            if(lexer->isError()){
                throw ParseException(*lexer->getError());
            }
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

        bool isNext(int32_t id){
            return lookB.id == id;
        }

        void fatal(){
            std::string str;

            if(lookA.value == nullptr){
                str = "\\0";
            }else{
                str = *lookA.value;
            }

            throw ParseException(Error(lookA.location, "wrong token " + str));
        }

        void fatal(Element* element){
            throw ParseException(Error(element->location, "wrong semantic"));
        }

        template<class T>
        T* create(){
            T* result = new T();
            result->location = lookA.location;
            return result;
        }

        Unit* parse(){
            Unit* unit = new Unit();
            for(;;){
                unit->children.push_back(parseExternalDeclaration());

                if(is(END_OF_FILE)){
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

                if(declaration->declarator != nullptr){
                    if( auto* paramDeclarator = dynamic_cast<ParameterDirectDeclarator*>(declaration->declarator->directDeclarator)){
                        if(auto* identifier = dynamic_cast<Identifier*>(paramDeclarator->directDeclarator)){
                            auto& def = defs[identifier->value];

                            auto &decls = paramDeclarator->parameterTypeList->declarations;

                            bool isFilling = is(LEFT_BRACE);

                            if(def.filled && isFilling){
                                fatal();
                            }

                            if(def.defined){
                                int minSize = std::min(decls.size(), def.types.size());

                                for( int i = 0 ; i < minSize ; i++ ){
                                    if( decls[i]->type->type != def.types[i]->type ){
                                        fatal(decls[i]->type);
                                    }
                                }

                                if( decls.size() < def.types.size() ){
                                    fatal();
                                }else if(decls.size() > def.types.size()){
                                    fatal(decls[minSize]->type);
                                }
                            }else{
                                for(auto* decl : decls){
                                    def.types.push_back(decl->type);
                                }

                                def.filled = isFilling;
                                def.defined = true;
                            }
                        }
                    }
                }

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
                auto type = create<Type>();
                switch(lookA.id){
                    case INT:
                        type->type = TYPE_INT;
                        break;
                    case CHAR:
                        type->type = TYPE_CHAR;
                        break;
                    case VOID:
                        type->type = TYPE_VOID;
                        break;
                }
                next();
                return type;
            }else if(is(STRUCT)){
                auto structType = new StructType();
                next();
                bool isIdentifier = is(IDENTIFIER);
                if(isIdentifier){
                    structType->name = lookA.value;
                    next();
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
                auto identifier = create<Identifier>();
                identifier->value = lookA.value;
                next();
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

            declaration->directDeclarator = parseDirectDeclarator(normal, abstract);

            if(declaration->pointer == 0 && declaration->directDeclarator == nullptr){
                return nullptr;
            }

            return declaration;
        }

        DirectDeclarator* parseDirectDeclarator(bool normal, bool abstract){
            DirectDeclarator* directDeclarator = nullptr;

            if(is(LEFT_PAREN)){
                if((!isType(lookB.id) && !isNext(RIGHT_PAREN)) || !abstract){
                    next();
                    directDeclarator = parseDeclarator(normal, abstract);
                    shall(RIGHT_PAREN);
                }
            }else if(normal && is(IDENTIFIER)){
                directDeclarator = parseIdentifier();
            }else if(!abstract){
                fatal();
            }

            while(eat(LEFT_PAREN)){
                auto inner = new ParameterDirectDeclarator();
                inner->directDeclarator = directDeclarator;
                inner->parameterTypeList = parseParameterTypeList(abstract);
                directDeclarator = inner;
                shall(RIGHT_PAREN);
            }

            return directDeclarator;
        }

        ParameterTypeList* parseParameterTypeList(bool nullable){
            auto parameterTypeList = new ParameterTypeList();

            if(!nullable || !is(RIGHT_PAREN)){
                while(true){
                    Type* type = parseType();
                    Declarator* declarator = parseDeclarator(true, true);

                    auto declaration = new Declaration();
                    declaration->type = type;
                    declaration->declarator = declarator;

                    parameterTypeList->declarations.push_back(declaration);

                    if(!eat(COMMA)){
                        break;
                    }
                }
            }


            return parameterTypeList;
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
            }else if(is(CONTINUE) ){
                if(!insideLoop){
                    fatal();
                }
                next();
                shall(SEMI);
                return new Continue();
            }else if(is(BREAK) ){
                if(!insideLoop){
                    fatal();
                }
                next();
                shall(SEMI);
                return new Break();
            }else if(is(IDENTIFIER) && isNext(COLON)){
                auto result = new LabeledStatement();
                result->name = lookA.value;
                eat(IDENTIFIER);
                eat(COLON);
                result->statement = parseStatement();
                return result;
            }else if(!eat(SEMI)){
                auto expr = parseExpression();
                shall(SEMI);
                return expr;
            }

            return nullptr;
        }

        GoTo* parseGoTo(){
            shall(GOTO);
            auto result = new GoTo();
            result->name = lookA.value;
            next();
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
                //while(eat(SEMI));

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
                result->hasFalse = true;
                result->falseBranch = parseStatement();
            }

            return result;
        }

        While* parseWhile(){
            bool beforeLoop = insideLoop;
            insideLoop = true;
            auto result = new While();
            shall(WHILE);
            shall(LEFT_PAREN);
            result->condition = parseExpression();
            shall(RIGHT_PAREN);
            result->body = parseStatement();
            insideLoop = beforeLoop;
            return result;
        }

        Expression* parseValue(){
            if(is(DIGIT_SEQUENCE)){
                auto number = new Number();
                number->value = lookA.value;
                next();
                return number;
            }else if(is(STRING_LITERAL)){
                auto string = new StringLiteral();
                string->value = lookA.value;
                next();
                return string;
            }else if(is(CONSTANT)){
                auto constant = new Constant();
                constant->value = lookA.value;
                next();
                return constant;
            }else if(is(IDENTIFIER)){
                return parseIdentifier();
            }

            fatal();
            return nullptr;
        }


        Expression* parseExpression(){
            return parseExpression(20);
        }

        Expression* parseExpression(uint32_t other){

            Expression* left = nullptr;
            uint32_t precedence = unary();

            if(precedence != 0){
                uint32_t op = lookA.id;
                next();

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
                precedence = leftPrecedence();
                if(precedence == 0 || precedence >= other){
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
                        bin->op = lookA.id;

                        if( bin->op == ASSIGN ){
                            if( const auto* leftBin = dynamic_cast<const Binary *>(left) ){
                                if(leftBin->op != ARROW && leftBin->op != DOT){
                                    fatal();
                                }
                            }else if( const auto* leftUnary = dynamic_cast<const Unary *>(left) ){
                                if(leftUnary->op != STAR){
                                    fatal();
                                }
                            }
                        }
                        next();
                        if (bin->op == ARROW || bin->op == DOT) {
                            bin->right = parseIdentifier();
                        } else {
                            bin->right = parseExpression(rightPrecedence(bin->op));
                        }
                        left = bin;
                    }
                }
            }
        }

        uint32_t leftPrecedence(){
            uint32_t index = 0;
            switch(lookA.id){
                case ARROW:
                case LEFT_PAREN:
                case LEFT_BRACKET:
                case DOT: index = 1; break;
                case STAR: index = 2; break;
                case PLUS:
                case MINUS: index = 4; break;
                case LESS:
                case LESS_EQUAL:
                case GREATER:
                case GREATER_EQUAL:
                case LEFT_SHIFT:
                case RIGHT_SHIFT: index = 6; break;
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

        uint32_t rightPrecedence(uint32_t id){
            uint32_t index = 0;
            switch(id){
                case ARROW:
                case LEFT_PAREN:
                case LEFT_BRACKET:
                case DOT: index = 1; break;
                case STAR: index = 2; break;
                case PLUS:
                case MINUS: index = 4; break;
                case LESS:
                case LESS_EQUAL:
                case GREATER:
                case GREATER_EQUAL:
                case LEFT_SHIFT:
                case RIGHT_SHIFT: index = 6; break;
                case EQUAL:
                case NOT_EQUAL: index = 7; break;
                case AND_AND: index = 11; break;
                case OR_OR: index = 12; break;
                case QUESTION: index = 13; break;
                case ASSIGN: index = 15; break;
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

