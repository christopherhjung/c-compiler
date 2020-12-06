//
// Created by Christopher Jung on 04.12.20.
//

#pragma once

#include "PrettyPrinter.h"
#include "TypeDefines.h"
#include <ostream>
#include <vector>
#include <iostream>

namespace parser{

    template<typename Base, typename T>
    inline bool instanceof(const T* obj) {
        return dynamic_cast<const Base *>(obj) != nullptr;
    }

    class Statement;
    void printIndentIfNotBlock(PrettyPrinter& stream, Statement* statement);
    void printStatement(PrettyPrinter& stream, Statement* statement);

    class Element{
    public:
        virtual ~Element() = default;
        virtual void dump(PrettyPrinter& stream) = 0;
    };
    class Declaration;

    class Unit : public Element{
    public:
        std::vector<Element*> children;

        void dump(PrettyPrinter& stream) override{
            bool first = true;
            for(auto child : children){
                if(first){
                    first = false;
                }else{
                    stream.newLine();
                    stream.newLine();
                }
                child->dump(stream);
                if(instanceof<Declaration>(child)){
                    stream << ";";
                }
            }
        }
    };

    class Statement : public Element{

    };

    class Expression : public Statement{

    };


    class Binary : public Expression{
    public:
        Expression* left = nullptr;
        Expression* right = nullptr;
        uint32_t op = 0;

        void dump(PrettyPrinter& stream) override{
            stream << "(";
            left->dump(stream);
            if(op != ARROW && op != DOT){
                stream << " ";
            }
            switch(op){
                case ARROW: stream << "->"; break;
                case DOT: stream << "."; break;

                case STAR: stream << "*"; break;
                case PLUS: stream << "+"; break;
                case MINUS: stream << "-"; break;
                case LESS: stream << "<"; break;
                case LESS_EQUAL: stream << "<="; break;
                case GREATER: stream << ">"; break;
                case GREATER_EQUAL: stream << ">="; break;
                case LEFT_SHIFT: stream << "<<"; break;
                case RIGHT_SHIFT: stream << ">>"; break;
                case EQUAL: stream << "=="; break;
                case NOT_EQUAL: stream << "!="; break;
                case AND_AND: stream << "&&"; break;
                case OR_OR: stream << "||"; break;
                case QUESTION: stream << "?"; break;
                case ASSIGN: stream << "="; break;
            }
            if(op != ARROW && op != DOT){
                stream << " ";
            }
            right->dump(stream);
            stream << ")";
        }
    };

    class Unary : public Expression{
    public:
        Expression* value = nullptr;
        uint32_t op = 0;

        void dump(PrettyPrinter& stream) override{
            stream << "(";
            switch(op){
                case SIZEOF: stream << "sizeof "; break;
                case AND: stream << "&"; break;
                case PLUS: stream << "+"; break;
                case MINUS: stream << "-"; break;
                case STAR: stream << "*"; break;
                case NOT: stream << "!"; break;
            }
            value->dump(stream);
            stream << ")";
        }
    };

    class Return : public Statement{
    public:
        Expression* value = nullptr;

        void dump(PrettyPrinter& stream) override{
            stream << "return ";
            value->dump(stream);
        }
    };

    class Type : public Element{

    public:
        uint32_t  type;

        void dump(PrettyPrinter& stream) override{
            if(type == INT){
                stream << "int";
            }else if(type == CHAR){
                stream << "char";
            }else if(type == VOID){
                stream << "void";
            }
        }
    };

    class DirectDeclarator : public Element{};
    class Identifier : public Expression, public DirectDeclarator{
    public:
        std::string value;

        void dump(PrettyPrinter& stream) override{
            stream << value;
        }
    };

    class StringLiteral : public Expression{
    public:
        std::string value;

        void dump(PrettyPrinter& stream) override{
            stream << value;
        }
    };

    class Constant : public Expression{
    public:
        std::string value;

        void dump(PrettyPrinter& stream) override{
            stream << value;
        }
    };

    class Number : public Expression{
    public:
        std::string value;

        void dump(PrettyPrinter& stream) override{
            stream << value;
        }
    };

    class If;
    class While;

    class Block : public Statement{
    public:
        std::vector<Statement*> children;

        void dump(PrettyPrinter& stream) override{
            stream << "{";
            stream.increaseDepth();
            stream.newLine();
            for(auto child : children){
                printStatement(stream, child);
                stream.newLine();
            }
            stream.decreaseDepth();
            stream << "}";
        }
    };

    class Continue : public Statement{
    public:

        void dump(PrettyPrinter& stream) override{
            stream << "continue";
        }
    };

    class Break : public Statement{
    public:

        void dump(PrettyPrinter& stream) override{
            stream << "break";
        }
    };

    class GoTo : public Statement{
    public:
        std::string name;

        void dump(PrettyPrinter& stream) override{
            stream << "goto " << name;
        }
    };


    class LabeledStatement : public Statement{
    public:
        Statement* statement = nullptr;
        std::string name;

        void dump(PrettyPrinter& stream) override{
            stream.withoutIndent();
            stream << name << ":";
            stream.newLine();
            if(statement != nullptr){
                statement->dump(stream);
            }
        }
    };

    class DirectDeclarator;

    class Declarator : public DirectDeclarator{
    public:
        int pointer = 0;
        DirectDeclarator* directDeclarator = nullptr;

        void dump(PrettyPrinter &stream) override;
    };

    class Declaration : public Statement{
    public:
        Type* type = nullptr;
        Declarator* declarator = nullptr;

        void dump(PrettyPrinter& stream) override{
            type->dump(stream);
            if(declarator != nullptr){
                stream << " ";
                declarator->dump(stream);
            }
        }
    };

    class ParameterTypeList : public Statement {
    public:
        std::vector<Declaration *> declarations;

        void dump(PrettyPrinter& stream) override{
            bool first = true;
            for(auto decl : declarations){
                if(first){
                    first = false;
                }else{
                    stream << ", ";
                }

                decl->dump(stream);
            }
        }
    };

    class ParameterDirectDeclarator : public DirectDeclarator{
    public:
        DirectDeclarator* directDeclarator = nullptr;
        ParameterTypeList* parameterTypeList = nullptr;

        void dump(PrettyPrinter& stream) override{
            stream << "(";
            if(directDeclarator != nullptr){
                directDeclarator->dump(stream);
            }

            if(parameterTypeList != nullptr){
                stream << "(";
                parameterTypeList->dump(stream);
                stream << ")";
            }
            stream << ")";
        }
    };

    class StructType : public Type{
    public:
        std::string name;
        std::vector<Declaration*> declarations;

        void dump(PrettyPrinter& stream) override{
            stream << "struct " << name;
            if(!declarations.empty()){
                stream.newLine();
                stream << "{";
                stream.newLine();
                stream.increaseDepth();
                for(auto decl : declarations){
                    decl->dump(stream);
                    stream << ";";
                    stream.newLine();
                }
                stream.decreaseDepth();
                stream << "}";
            }
        }
    };

    class Method : public Element{
    public:
        Declaration* declaration = nullptr;
        Block* body = nullptr;

        void dump(PrettyPrinter& stream) override{
            declaration->dump(stream);
            stream.newLine();
            body->dump(stream);
        }
    };

    class Call : public Expression{
    public:
        Expression* target = nullptr;
        std::vector<Expression*> values;

        void dump(PrettyPrinter& stream) override{
            stream << "(";
            target->dump(stream);
            stream << "(";
            bool first = true;
            for(auto val : values){
                if(first){
                    first = false;
                }else{
                    stream << ", ";
                }
                val->dump(stream);
            }
            stream << ")";
            stream << ")";
        }
    };

    class Sizeof : public Expression{
    public:
        Type* type = nullptr;
        Declarator *declarator = nullptr;

        void dump(PrettyPrinter& stream) override{
            stream << "(";
            stream << "sizeof(";
            type->dump(stream);
            if(declarator != nullptr){
                declarator->dump(stream);
            }
            stream << ")";
            stream << ")";
        }
    };

    class Select : public Expression{
    public:
        Expression* target = nullptr;
        Expression* index = nullptr;

        void dump(PrettyPrinter& stream) override{
            stream << "(";
            target->dump(stream);
            stream << "[";
            index->dump(stream);
            stream << "]";
            stream << ")";
        }
    };

    class IfSmall : public Expression{
    public:
        Expression* condition = nullptr;
        Expression* left = nullptr;
        Expression* right = nullptr;

        void dump(PrettyPrinter& stream) override{
            stream << "(";
            condition->dump(stream);
            stream << "?";
            left->dump(stream);
            stream << ":";
            right->dump(stream);
            stream << ")";
        }
    };


    class If : public Statement{
    public:
        Expression* condition = nullptr;
        Statement* trueBranch = nullptr;
        Statement* falseBranch = nullptr;

        void dump(PrettyPrinter& stream) override{
            stream << "if (";
            condition->dump(stream);
            stream << ")";

            printIndentIfNotBlock(stream, trueBranch);

            if(falseBranch != nullptr){
                if(instanceof<Block>(trueBranch)){
                    stream << " ";
                }else{
                    stream.newLine();
                }

                stream << "else";

                if(instanceof<If>(falseBranch)){
                    stream << " ";
                    falseBranch->dump(stream);
                }else{
                    printIndentIfNotBlock(stream, falseBranch);
                }
            }
        }
    };

    class While : public Statement{
    public:
        Expression* condition = nullptr;
        Statement* body = nullptr;

        void dump(PrettyPrinter& stream) override{
            stream << "while (";
            condition->dump(stream);
            stream << ")";
            printIndentIfNotBlock(stream, body);
        }
    };
}