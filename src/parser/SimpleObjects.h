//
// Created by Christopher Jung on 04.12.20.
//

#pragma once

#include "PrettyPrinter.h"
#include "TypeDefines.h"
#include "../lexer/Location.h"
//#include "Visitor.h"
#include <ostream>
#include <vector>
#include <iostream>
#include "unordered_map"
#include "unordered_set"
#include "../utils/Comparable.h"
#include "../semantic/SemanticException.h"

template<typename Base, typename T>
inline bool instanceof(const T* obj) {
    return dynamic_cast<const Base *>(obj) != nullptr;
}

class SuperType;
class Statement;

void printIndentIfNotBlock(PrettyPrinter& printer, Statement* statement);
void printStatement(PrettyPrinter& printer, Statement* statement);

class Dumpable{
public:
    virtual void dump(PrettyPrinter& printer) = 0;
    //virtual void visit(Visitor& visitor) = 0;
};

class Element : public Dumpable{
public:
    Location location;
    virtual ~Element() = default;
};
class Declaration;

class Unit : public Element{
public:
    std::vector<Element*> children;

    void dump(PrettyPrinter& printer) override{
        bool first = true;
        for(auto child : children){
            if(first){
                first = false;
            }else{
                printer.newLine();
                printer.newLine();
            }
            child->dump(printer);
            if(instanceof<Declaration>(child)){
                printer << ";";
            }
        }
    }
};

class Statement : public Element{

};

class Expression : public Statement{
public:
    Declaration* type = nullptr;
    const SuperType* superType = nullptr;

    Declaration* getType(){
        return type;
    };
};

class Operator : public Element{
public:
    uint32_t id;

    void dump(PrettyPrinter& printer) override{
        switch(id){
            case ARROW: printer << "->"; break;
            case DOT: printer << "."; break;

            case STAR: printer << "*"; break;
            case PLUS: printer << "+"; break;
            case MINUS: printer << "-"; break;
            case LESS: printer << "<"; break;
            case LESS_EQUAL: printer << "<="; break;
            case GREATER: printer << ">"; break;
            case GREATER_EQUAL: printer << ">="; break;
            case LEFT_SHIFT: printer << "<<"; break;
            case RIGHT_SHIFT: printer << ">>"; break;
            case EQUAL: printer << "=="; break;
            case NOT_EQUAL: printer << "!="; break;
            case AND_AND: printer << "&&"; break;
            case OR_OR: printer << "||"; break;
            case QUESTION: printer << "?"; break;
            case ASSIGN: printer << "="; break;

            case SIZEOF: printer << "sizeof "; break;
            case AND: printer << "&"; break;
            case NOT: printer << "!"; break;
        }
    }
};

class Binary : public Expression{
public:
    Expression* left = nullptr;
    Expression* right = nullptr;
    Operator* op = nullptr;

    void dump(PrettyPrinter& printer) override{
        printer << "(";
        left->dump(printer);
        if(op->id != ARROW && op->id != DOT){
            printer << " ";
        }
        op->dump(printer);
        if(op->id != ARROW && op->id != DOT){
            printer << " ";
        }
        right->dump(printer);
        printer << ")";
    }
};

class Unary : public Expression{
public:
    Expression* value = nullptr;
    Operator* op = nullptr;

    void dump(PrettyPrinter& printer) override{
        printer << "(";
        op->dump(printer);
        value->dump(printer);
        printer << ")";
    }
};

class Return : public Statement{
public:
    Expression* value = nullptr;

    void dump(PrettyPrinter& printer) override{
        printer << "return";
        if(value != nullptr){
            printer << " ";
            value->dump(printer);
        }
    }
};

class Type : public Element{

public:
    uint32_t  type;

    void dump(PrettyPrinter& printer) override{
        if(type == TYPE_INT){
            printer << "int";
        }else if(type == TYPE_CHAR){
            printer << "char";
        }else if(type == TYPE_VOID){
            printer << "void";
        }
    }
};

class DirectDeclarator : public Dumpable{};
class Identifier : public Expression, public DirectDeclarator{
public:
    const std::string *value = nullptr;

    void dump(PrettyPrinter& printer) override{
        printer << *value;
    }

    bool operator==(const Identifier &rhs) const {
        return value == rhs.value;
    }

    bool operator!=(const Identifier &rhs) const {
        return !(rhs == *this);
    }
};

class StringLiteral : public Expression{
public:
    const std::string *value = nullptr;

    void dump(PrettyPrinter& printer) override{
        printer << *value;
    }
};

class Constant : public Expression{
public:
    const std::string *value = nullptr;

    void dump(PrettyPrinter& printer) override{
        printer << *value;
    }
};

class Number : public Expression{
public:
    const std::string *value = nullptr;

    void dump(PrettyPrinter& printer) override{
        printer << *value;
    }
};

class If;
class While;

class Block : public Statement{
public:
    std::vector<Statement*> children;

    void dump(PrettyPrinter& printer) override{
        printer << "{";
        printer.increaseDepth();
        printer.newLine();
        for(auto child : children){
            printStatement(printer, child);
            printer.newLine();
        }
        printer.decreaseDepth();
        printer << "}";
    }
};

class Continue : public Statement{
public:

    void dump(PrettyPrinter& printer) override{
        printer << "continue";
    }
};

class Break : public Statement{
public:

    void dump(PrettyPrinter& printer) override{
        printer << "break";
    }
};

class GoTo : public Statement{
public:
    const std::string *name;

    void dump(PrettyPrinter& printer) override{
        printer << "goto " << *name;
    }
};


class LabeledStatement : public Statement{
public:
    Statement* statement = nullptr;
    const std::string *name = nullptr;

    void dump(PrettyPrinter& printer) override{
        printer.withoutIndent();
        printer << *name << ":";
        printer.newLine();
        printStatement(printer, statement);
    }
};

class DirectDeclarator;

class Declarator : public DirectDeclarator{
public:
    int pointer = 0;
    DirectDeclarator* directDeclarator = nullptr;

    void dump(PrettyPrinter &printer) override;
};

class Declaration : public Statement{
public:
    Type* type = nullptr;
    Declarator* declarator = nullptr;

    void dump(PrettyPrinter& printer) override{
        type->dump(printer);
        if(declarator != nullptr){
            printer << " ";
            declarator->dump(printer);
        }
    }
};

class ParameterTypeList : public Statement {
public:
    std::vector<SuperType*> declarations2;
    std::vector<Declaration *> declarations;

    void dump(PrettyPrinter& printer) override{
        bool first = true;
        for(auto decl : declarations){
            if(first){
                first = false;
            }else{
                printer << ", ";
            }

            decl->dump(printer);
        }
    }
};

class ParameterDirectDeclarator : public DirectDeclarator{
public:
    DirectDeclarator* directDeclarator = nullptr;
    ParameterTypeList* parameterTypeList = nullptr;

    void dump(PrettyPrinter& printer) override{
        printer << "(";
        if(directDeclarator != nullptr){
            directDeclarator->dump(printer);
        }

        if(parameterTypeList != nullptr){
            printer << "(";
            parameterTypeList->dump(printer);
            printer << ")";
        }
        printer << ")";
    }
};

class StructType : public Type{
public:
    const std::string *name;
    std::vector<Declaration*> declarations;

    void dump(PrettyPrinter& printer) override{
        printer << "struct";

        if(name != nullptr){
            printer << " " << *name;
        }

        if(!declarations.empty()){
            printer.newLine();
            printer << "{";
            printer.newLine();
            printer.increaseDepth();
            for(auto decl : declarations){
                decl->dump(printer);
                printer << ";";
                printer.newLine();
            }
            printer.decreaseDepth();
            printer << "}";
        }
    }
};

class Method : public Element{
public:
    Declaration* declaration = nullptr;
    SuperType* type = nullptr;
    Block* body = nullptr;

    void dump(PrettyPrinter& printer) override{
        declaration->dump(printer);
        printer.newLine();
        body->dump(printer);
    }
};

class Call : public Expression{
public:
    Expression* target = nullptr;
    std::vector<Expression*> values;

    void dump(PrettyPrinter& printer) override{
        printer << "(";
        target->dump(printer);
        printer << "(";
        bool first = true;
        for(auto val : values){
            if(first){
                first = false;
            }else{
                printer << ", ";
            }
            val->dump(printer);
        }
        printer << ")";
        printer << ")";
    }
};

class Sizeof : public Expression{
public:
    Type* type = nullptr;
    Declarator *declarator = nullptr;

    void dump(PrettyPrinter& printer) override{
        printer << "(";
        printer << "sizeof(";
        type->dump(printer);
        if(declarator != nullptr){
            declarator->dump(printer);
        }
        printer << ")";
        printer << ")";
    }
};

class Select : public Expression{
public:
    Expression* target = nullptr;
    Expression* index = nullptr;

    void dump(PrettyPrinter& printer) override{
        printer << "(";
        target->dump(printer);
        printer << "[";
        index->dump(printer);
        printer << "]";
        printer << ")";
    }
};

class IfSmall : public Expression{
public:
    Expression* condition = nullptr;
    Expression* left = nullptr;
    Expression* right = nullptr;

    void dump(PrettyPrinter& printer) override{
        printer << "(";
        condition->dump(printer);
        printer << " ? ";
        left->dump(printer);
        printer << " : ";
        right->dump(printer);
        printer << ")";
    }
};


class If : public Statement{
public:
    Expression* condition = nullptr;
    Statement* trueBranch = nullptr;
    bool hasFalse = false;
    Statement* falseBranch = nullptr;

    void dump(PrettyPrinter& printer) override{
        printer << "if (";
        condition->dump(printer);
        printer << ")";

        printIndentIfNotBlock(printer, trueBranch);

        if(hasFalse){
            if(instanceof<Block>(trueBranch)){
                printer << " ";
            }else{
                printer.newLine();
            }

            printer << "else";

            if(instanceof<If>(falseBranch)){
                printer << " ";
                falseBranch->dump(printer);
            }else{
                printIndentIfNotBlock(printer, falseBranch);
            }
        }
    }
};

class While : public Statement{
public:
    Expression* condition = nullptr;
    Statement* body = nullptr;

    void dump(PrettyPrinter& printer) override{
        printer << "while (";
        condition->dump(printer);
        printer << ")";
        printIndentIfNotBlock(printer, body);
    }
};

class SuperType : public Comparable{
public:
    const Identifier* identifier = nullptr;
    virtual const SuperType* apply(const Expression* expression) const = 0;
    virtual bool isSimple() const = 0;
    virtual bool isPointer() const = 0;
};

class ComplexType : public SuperType{
public:
    const SuperType* subType = nullptr;
    explicit ComplexType(const SuperType* subType) : subType(subType){

    }
};

class MethodType : public ComplexType{
public:
    std::vector<SuperType*> types;

    explicit MethodType(const SuperType* subType) : ComplexType(subType) {

    }

    const SuperType* apply(const Expression* expression) const override {
        if(auto call = dynamic_cast<const Call*>(expression)){
            if( types.size() != call->values.size() ){

            }
            int min = std::min(types.size() , call->values.size());

            for( int i = 0 ; i < min ; i++ ){
                if( !types[i]->equals(call->values[i]->superType) ){
                    return nullptr;
                    //throw SemanticException(call->values[i]->location);
                }
            }

            /*if(types.size() < call->values.size()){
                throw SemanticException(call->values[min]->location);
            }else if(types.size() > call->values.size()){
                throw SemanticException(call->values[0]->location);
            }*/

            return subType;
        }

        return nullptr;
        //throw SemanticException(expression->location);
    }

    uint64_t hash() const override {
        return 0;
    }

    bool equals(const Comparable *other) const override {
        if( auto superType = dynamic_cast<const MethodType*>(other) ){
            if( types.size() != superType->types.size() ){
                return false;
            }

            for( int i = 0 ; i < superType->types.size() ; i++ ){
                if( !types[i]->equals(superType->types[i]) ){
                    return false;
                }
            }

            return true;
        }
        return false;
    }

    bool isSimple() const override {
        return false;
    }

    bool isPointer() const override {
        return false;
    }
};

class DeallocateType : public ComplexType{
public:
    explicit DeallocateType(const SuperType* subType) : ComplexType(subType) {

    }

    uint64_t hash() const override {
        return 0;
    }

    bool equals(const Comparable *other) const override {
        if( auto simpleOther = dynamic_cast<const DeallocateType*>(other) ){
            return subType->equals(simpleOther->subType);
        }
        return false;
    }

    const SuperType* apply(const Expression* instruction) const override {
        return subType;
    }

    bool isSimple() const override {
        return false;
    }

    bool isPointer() const override {
        return true;
    }
};

class SuperStructType : public SuperType{
public:
    std::unordered_map<const std::string*, int> map;
    std::vector<const SuperType*> types;

    explicit SuperStructType() {

    }

    uint64_t hash() const override {
        return 0;
    }

    bool equals(const Comparable *other) const override {
        if(auto structType = dynamic_cast<const SuperStructType*>(other)){
            if(types.size() != structType->types.size()){
                return false;
            }

            for(int i = 0 ; i < types.size() ; i++){
                auto left = types[i];
                auto right = structType->types[i];

                if( !left->equals(right) ){
                    return false;
                }
            }

            return true;
        }
        return false;
    }

    const SuperType* apply(const Expression* instruction) const override {
        if(auto identifier = dynamic_cast<const Identifier*>(instruction)){
            auto pos = map.find(identifier->value);

            if(pos == map.end()){
                return nullptr;
            }

            return types[pos->second];
        }

        return nullptr;
    }

    bool isSimple() const override {
        return true;
    }

    bool isPointer() const override {
        return false;
    }
};

class SimpleType : public SuperType{
public:
    int id;

    explicit SimpleType(int id) : id(id){

    }

    uint64_t hash() const override {
        return 0;
    }

    bool equals(const Comparable *other) const override {
        if(auto otherSimple = dynamic_cast<const SimpleType*>(other)){
            return id == otherSimple->id;
        }
        return false;
    }

    const SuperType* apply(const Expression* instruction) const override {
        return nullptr;
    }

    bool isSimple() const override {
        return true;
    }

    bool isPointer() const override {
        return false;
    }
};