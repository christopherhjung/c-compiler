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
#include "../transform/TransformException.h"

#include "llvm/IR/Module.h"                /* Module */
#include "llvm/IR/Function.h"              /* Function */
#include "llvm/IR/IRBuilder.h"             /* IRBuilder */
#include "llvm/IR/LLVMContext.h"           /* LLVMContext */
#include "llvm/IR/GlobalValue.h"           /* GlobaleVariable, LinkageTypes */
#include "llvm/IR/Verifier.h"              /* verifyFunction, verifyModule */
#include "llvm/Support/Signals.h"          /* Nice stacktrace output */
#include "llvm/Support/SystemUtils.h"
#include "llvm/Support/PrettyStackTrace.h"
#include "../transform/TransformContext.h"



#include <sstream>

template<typename Base, typename T>
inline bool instanceof(const T* obj) {
    return dynamic_cast<const Base *>(obj) != nullptr;
}

class SuperType;
class Statement;
class If;
class While;
class DirectDeclarator;

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
    std::string toString(){
        std::stringstream ss;
        PrettyPrinter prettyPrinter(ss);
        dump(prettyPrinter);
        return ss.str();
    }
};
class Declaration;


class Statement : public Element{
public:
    const SuperType* superType = nullptr;

    virtual llvm::Value* create(TransformContext &context){
        TRANSFORM_ERROR();
    }

    virtual llvm::BasicBlock* create(TransformContext &context, llvm::BasicBlock* start){
        TRANSFORM_ERROR();
    }
};

class Expression : public Statement{
public:
    virtual llvm::Value* createRightValue(TransformContext &context){
        TRANSFORM_ERROR();
    };

    virtual llvm::Value* createLeftValue(TransformContext &context){
        TRANSFORM_ERROR();
    };

    virtual void createConditionBranch(TransformContext& context, llvm::BasicBlock* activeBlock, llvm::BasicBlock* trueBlock, llvm::BasicBlock* falseBlock){
        context.builder.SetInsertPoint(activeBlock);
        llvm::Value *condition = context.builder.CreateICmpNE(createRightValue(context), context.builder.getInt32(0));
        context.builder.CreateCondBr(condition, trueBlock, falseBlock);
    }
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

class CompareBinary : public Binary{
public:
    void createConditionBranch(TransformContext& context, llvm::BasicBlock* activeBlock, llvm::BasicBlock* trueBlock, llvm::BasicBlock* falseBlock) override {
        context.builder.SetInsertPoint(activeBlock);
        llvm::Value *condition = context.builder.CreateICmpNE(createRightValue(context), context.builder.getInt32(0), "condition");
        context.builder.CreateCondBr(condition, trueBlock, falseBlock);
    }

    llvm::CmpInst::Predicate getPredicate(){
        llvm::CmpInst::Predicate predicate = llvm::CmpInst::Predicate::BAD_ICMP_PREDICATE;
        switch(this->op->id){

            case EQUAL: predicate = llvm::CmpInst::Predicate::ICMP_EQ; break;
            case NOT_EQUAL: predicate = llvm::CmpInst::Predicate::ICMP_NE; break;
            case LESS: predicate = llvm::CmpInst::Predicate::ICMP_SLT; break;
            case LESS_EQUAL: predicate = llvm::CmpInst::Predicate::ICMP_SLE; break;
            case GREATER: predicate = llvm::CmpInst::Predicate::ICMP_SGT; break;
            case GREATER_EQUAL: predicate = llvm::CmpInst::Predicate::ICMP_SGE; break;
        }

        return predicate;
    }

    llvm::Value* createRightValue(TransformContext &context) override{
        return context.builder.CreateICmp(getPredicate(),left->create(context), right->create(context));
    }
};

class Assignment : public Binary{
public:
    llvm::Value* createRightValue(TransformContext &context) override{
        return context.builder.CreateStore(right->createRightValue(context), left->createLeftValue(context));
    }

    llvm::Value* create(TransformContext &context) override {
        TRANSFORM_ERROR();
    }

    llvm::BasicBlock* create(TransformContext &context, llvm::BasicBlock* start) override{
        context.builder.SetInsertPoint(start);
        createRightValue(context);
        return start;
    }
};

class LogicalAnd : public Binary{
public:
    void createConditionBranch(TransformContext& context, llvm::BasicBlock* activeBlock, llvm::BasicBlock* trueBlock, llvm::BasicBlock* falseBlock) override {
        auto extraBB = context.createBasicBlock("logic-and-temp");
        left->createConditionBranch(context,activeBlock, extraBB, falseBlock);
        right->createConditionBranch(context, extraBB, trueBlock, falseBlock);
    }

    llvm::Value* createLeftValue(TransformContext &context) override{

    }

    llvm::Value* createRightValue(TransformContext &context) override{

    }
};

class LogicalOr : public Binary{
public:
    void createConditionBranch(TransformContext& context, llvm::BasicBlock* activeBlock, llvm::BasicBlock* trueBlock, llvm::BasicBlock* falseBlock) override {
        auto extraBB = context.createBasicBlock("logic-and-temp");
        left->createConditionBranch(context,activeBlock, trueBlock, extraBB);
        right->createConditionBranch(context, extraBB, trueBlock, falseBlock);
    }

    llvm::Value* createLeftValue(TransformContext &context) override{

    }

    llvm::Value* createRightValue(TransformContext &context) override{

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

    Identifier(){

    }

    Identifier(const std::string* value) : value(value){

    }

    void dump(PrettyPrinter& printer) override{
        printer << *value;
    }

    bool operator==(const Identifier &rhs) const {
        return value == rhs.value;
    }

    bool operator!=(const Identifier &rhs) const {
        return !(rhs == *this);
    }

    llvm::Value* createRightValue(TransformContext &context) override{
        llvm::Value *randomVariable = context.resetAllocBuilder().CreateAlloca(context.builder.getInt32Ty());
        return context.builder.CreateLoad(randomVariable);
    }

    llvm::Value* createLeftValue(TransformContext &context) override {
        llvm::Value *randomVariable = context.resetAllocBuilder().CreateAlloca(context.builder.getInt32Ty());
        return randomVariable;
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

    llvm::Value* createRightValue(TransformContext &context) override{
        return context.builder.getInt32(1);
    }
};

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

    llvm::BasicBlock* create(TransformContext &context, llvm::BasicBlock *start) override{
        for( auto child  : children ){
            start = child->create(context, start);
        }

        return start;
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

    virtual llvm::Value* create(TransformContext &context){
        

        return context.resetAllocBuilder().CreateAlloca(context.builder.getInt32Ty());
    }

    virtual llvm::BasicBlock* create(TransformContext &context, llvm::BasicBlock* start){
        context.builder.SetInsertPoint(start);
        create(context);
        return start;
    }
};

class ParameterTypeList : public Dumpable {
public:
    std::vector<Location> locations;
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
    Block* body = nullptr;

    void dump(PrettyPrinter& printer) override{
        declaration->dump(printer);
        printer.newLine();
        body->dump(printer);
    }

    void create(TransformContext& context){
        auto entry = context.createFunction("main");
        body->create(context, entry);
    }
};

class Call : public Expression{
public:
    Expression* target = nullptr;
    std::vector<Expression*> values;
    std::vector<Location> locations;

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

    llvm::BasicBlock* create(TransformContext &context, llvm::BasicBlock* start) override{
        llvm::BasicBlock *conditionBlock = context.createBasicBlock("if-condition");
        llvm::BasicBlock *startTrue = context.createBasicBlock("if-true");
        llvm::BasicBlock *end = context.createBasicBlock("if-end");

        context.builder.SetInsertPoint(start);
        context.builder.CreateBr(conditionBlock);

        llvm::BasicBlock *startFalse;
        llvm::BasicBlock *endTrueBlock = trueBranch->create(context, startTrue);
        context.builder.SetInsertPoint(endTrueBlock);
        context.builder.CreateBr(end);

        if(falseBranch == nullptr){
            startFalse = end;
        }else{
            startFalse = context.createBasicBlock("if-false");
            llvm::BasicBlock *endFalse = falseBranch->create(context, startFalse);
            context.builder.SetInsertPoint(endFalse);
            context.builder.CreateBr(end);
        }

        condition->createConditionBranch(context,conditionBlock, startTrue, startFalse);

        return end;
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

    void create(TransformContext& context){
        for( auto element : children ){
            if( auto method = dynamic_cast<Method*>(element) ){
                method->create(context);
            }else if( auto decl = dynamic_cast<Declaration*>(element) ){

            }
        }
    }
};

class MethodType;
class PointerType;
class SuperStructType;
class SimpleType;

class SuperType : public Comparable<SuperType>{
public:
    bool assignable = false;
    const Identifier* identifier = nullptr;

    SuperType(){

    }

    SuperType(bool assignable) : assignable(assignable){

    }

    virtual const MethodType* asMethodType() const{
        return nullptr;
    }

    virtual const PointerType* asPointerType() const{
        return nullptr;
    }

    virtual const SuperStructType* asSuperStructType() const{
        return nullptr;
    }

    virtual const SimpleType* asSimpleType() const{
        return nullptr;
    }

    virtual bool isAssignable() const{
        return assignable;
    }
};

class ComplexType : public SuperType{
public:
    const SuperType* subType = nullptr;
    explicit ComplexType(const SuperType* subType) : subType(subType){

    }

    explicit ComplexType(const SuperType* subType, bool assignable) : SuperType(assignable), subType(subType){

    }
};

class MethodType : public ComplexType{
public:
    std::vector<SuperType*> types;
    std::vector<Location> locations;

    explicit MethodType(const SuperType* subType) : ComplexType(subType) {

    }

    const SuperType* call(const Call* call) const {
        if(call){
            if( types.size() != call->values.size() ){
                return nullptr;
            }
            int min = std::min(types.size() , call->values.size());

            for( int i = 0 ; i < min ; i++ ){
                if( !types[i]->equals(call->values[i]->superType) ){
                    return nullptr;
                }
            }

            return subType;
        }

        return nullptr;
    }

    bool equals(const SuperType *other) const override {
        if( auto superType =other->asMethodType() ){
            if(types.empty() || superType->types.empty()){
                return true;
            }

            if( types.size() != superType->types.size() ){
                return false;
            }

            for( unsigned long i = 0 ; i < superType->types.size() ; i++ ){
                if( !types[i]->equals(superType->types[i]) ){
                    return false;
                }
            }

            return true;
        }
        return false;
    }

    const MethodType* asMethodType() const override{
        return this;
    }
};

class PointerType : public ComplexType{
public:
    explicit PointerType(const SuperType* subType) : ComplexType(subType) {

    }

    explicit PointerType(const SuperType* subType, bool assignable) : ComplexType(subType, assignable) {

    }

    bool equals(const SuperType *other) const override {
        if( auto simpleOther = other->asPointerType() ){
            return subType->equals(simpleOther->subType);
        }
        return false;
    }

    const PointerType* asPointerType() const override{
        return this;
    }
};



class SuperStructType : public SuperType{
public:
    std::unordered_map<const std::string*, int> map;
    std::vector<const SuperType*> types;

    explicit SuperStructType() {

    }

    explicit SuperStructType(bool assignable) : SuperType(assignable) {

    }

    bool equals(const SuperType *other) const override {
        if(auto structType = other->asSuperStructType()){
            /*if(types.size() != structType->types.size()){
                return false;
            }

            for(int i = 0 ; i < types.size() ; i++){
                auto left = types[i];
                auto right = structType->types[i];

                if( !left->equals(right) ){
                    return false;
                }
            }*/

            return structType == this;
        }
        return false;
    }

    const SuperType* member(const Identifier* identifier) const {
        if(identifier){
            auto pos = map.find(identifier->value);

            if(pos == map.end()){
                return nullptr;
            }

            return types[pos->second];
        }

        return nullptr;
    }

    const SuperStructType* asSuperStructType() const override{
        return this;
    }
};

class SimpleType : public SuperType{
public:
    int id;

    explicit SimpleType(int id) : id(id){

    }

    explicit SimpleType(int id, bool assignable) : SuperType(assignable), id(id){

    }

    bool equals(const SuperType *other) const override {
        if(auto otherSimple = other->asSimpleType()){
            return id == otherSimple->id;
        }
        return false;
    }

    const SimpleType* asSimpleType() const override{
        return this;
    }
};

class ProxyType : public SuperType{
public:
    const SuperType* inner;

    explicit ProxyType(const SuperType* inner, bool assignable) : SuperType(assignable), inner(inner){

    }

    bool equals(const SuperType *other) const override {
        return inner->equals(other);
    }

    const SimpleType* asSimpleType() const override{
        return inner->asSimpleType();
    }

    const MethodType* asMethodType() const override{
        return inner->asMethodType();
    }

    const PointerType* asPointerType() const override{
        return inner->asPointerType();
    }

    const SuperStructType* asSuperStructType() const override{
        return inner->asSuperStructType();
    }
};