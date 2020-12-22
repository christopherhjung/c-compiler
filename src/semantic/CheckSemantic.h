//
// Created by Christopher Jung on 21.12.20.
//

#pragma once


#include "../parser/SimpleObjects.h"
#include "../lexer/Location.h"
#include "SemanticException.h"


extern SimpleType* const IntType = new SimpleType(TYPE_INT);
extern SimpleType* const CharType = new SimpleType(TYPE_CHAR);
extern SimpleType* const VoidType = new SimpleType(TYPE_VOID);
extern SimpleType* const BoolType = new SimpleType(TYPE_BOOL);
extern SimpleType* const StringType = new SimpleType(TYPE_STRING);

#define ERROR(LOCATION) (throw SemanticException(LOCATION, __LINE__))

template<class T>
class Descriptor{
public:
    T* superType = nullptr;
    bool filled = false;
    bool defined = false;

    Descriptor(){

    }

    Descriptor(const Descriptor* other) : superType(other->superType), filled(other->filled), defined(other->defined){

    }

    Descriptor(SuperType* superType) : superType(superType){

    }
};

class Scope{
public:
    std::unordered_map<const std::string*, Descriptor<SuperType>> types;
    std::unordered_map<const std::string*, const std::string*> labels;
    std::unordered_map<const std::string*, Descriptor<SuperStructType>> structs;
    Scope* parent = nullptr;
    const SuperType* returnType;
    bool overridePermitted = false;

    Scope() : Scope(false){

    }

    Scope( bool overridePermitted ) : overridePermitted(overridePermitted){

    }

    const SuperType* getReturnType(){
        if(returnType != nullptr){
            return returnType;
        }else if(parent != nullptr){
            return parent->getReturnType();
        }

        return nullptr;
    }

    Descriptor<SuperStructType>* getStruct(const std::string* str){
        auto current = structs.find(str);
        if( current != structs.end() ){
            return &current->second;
        }else if( parent != nullptr ){
            return parent->getStruct(str);
        }

        return nullptr;
    }

    bool setStruct(const std::string* str, SuperStructType* type){
        auto desc = &structs[str];

        if( desc->defined ){
            if(!(overridePermitted && desc->superType->equals(type))){
                return false;
            }
        }else{
            desc->superType = type;
            desc->defined = true;
        }

        return true;
    }

    bool set(const std::string* str, SuperType* type){
        auto desc = &types[str];

        if( desc->defined ){
            if(!(overridePermitted && desc->superType->equals(type))){
                return false;
            }
        }else{
            desc->superType = type;
            desc->defined = true;
        }

        return true;
    }

    Descriptor<SuperType>* get(const std::string* str){
        if( types.find(str) != types.end() ){
            return &types[str];
        }else if( parent != nullptr ){
            return parent->get(str);
        }else{
            return &types[str];
        }
    }
};

class Semantic {
public:
    Scope mainScope{true};
    Scope* currentScope = &mainScope;

    void fatal(Element* element){
        Location& loc = element->location;

        ERROR(loc);
    }

    void check(Unit *element) {
        for (auto child : element->children) {
            if (auto *declaration = dynamic_cast<Declaration *>(child)) {
                enter0(declaration);
            } else if (auto *method = dynamic_cast<Method *>(child)) {
                auto methodType = static_cast<MethodType*>(enter(method->declaration));

                if(methodType->identifier != nullptr && !currentScope->set(methodType->identifier->value, methodType)){
                    currentScope->set(methodType->identifier->value, methodType);
                    ERROR(methodType->identifier->location);
                }

                auto inner = new Scope();
                if(methodType->types.size() != 1 || !methodType->types[0]->equals(VoidType)){
                    for( auto paramType : methodType->types ){
                        auto identifier = paramType->identifier;

                        if(identifier == nullptr){
                            //ERROR(Location());
                        }else{
                            if(!inner->set(identifier->value, paramType)){
                                ERROR(identifier->location);
                            }
                        }
                    }
                }


                auto carry = currentScope;
                currentScope = inner;
                inner->parent = carry;
                inner->returnType = methodType->subType;
                enter(method->body);
                currentScope = carry;
            }
        }
    }

    void enter(Block* block){
        if( block != nullptr ){
            Scope* carry = currentScope;
            currentScope = new Scope();
            currentScope->parent = carry;
            for( auto statement : block->children){
                enter(statement);
            }
            currentScope = carry;
        }
    }

    void enter(Statement* statement){
        if(statement == nullptr){
            return;
        }else if(auto labeledStatement = dynamic_cast<LabeledStatement*>(statement)){
            enter(labeledStatement);
        }else if(auto ifStatement = dynamic_cast<If*>(statement)){
            enter(ifStatement);
        }else if(auto whileStatement = dynamic_cast<While*>(statement)){
            enter(whileStatement);
        }else if(auto returnStatement = dynamic_cast<Return*>(statement)){
            enter(returnStatement);
        }else if(auto gotoStatement = dynamic_cast<GoTo*>(statement)){
            enter(gotoStatement);
        }else if(auto continueStatement = dynamic_cast<Continue*>(statement)){
            enter(continueStatement);
        }else if(auto breakStatement = dynamic_cast<Break*>(statement)){
            enter(breakStatement);
        }else if(auto block = dynamic_cast<Block*>(statement)){
            enter(block);
        }else if(auto declaration = dynamic_cast<Declaration*>(statement)){
            enter0(declaration);
        }else if(auto expression = dynamic_cast<Expression*>(statement)){
            enter(expression);
        }else{
            ERROR(statement->location);
        }
    }

    void enter(LabeledStatement* labeledStatement){
        enter(labeledStatement->statement);

        currentScope->labels[labeledStatement->name] = labeledStatement->name;
    }

    void enter(If* ifStatement){
        enter(ifStatement->condition);
        enter(ifStatement->trueBranch);
        enter(ifStatement->falseBranch);

        if( !ifStatement->condition->superType->equals(IntType) ){
            ERROR(Location());
        }
    }

    void enter(While* whileStatement){
        enter(whileStatement->condition);
        enter(whileStatement->body);

        if( !whileStatement->condition->superType->equals(IntType) ){
            ERROR(Location());
        }
    }

    void enter(GoTo* gotoStatement){

    }

    void enter(Return* returnStatement){
        enter(returnStatement->value);

        Element* loc;
        const SuperType* returnType;
        if( returnStatement->value == nullptr ){
            returnType = VoidType;
            loc = returnStatement;
        }else{
            loc = returnStatement->value;
            returnType = returnStatement->value->superType;
        }

        if( !returnType->equals(currentScope->getReturnType()) ){
            ERROR(loc->location);
        }
    }

    void enter(Continue* continueStatement){

    }

    void enter(Break* breakStatement){

    }

    void enter(Expression* expression){
        if(expression == nullptr){
            return;
        }else if(auto binary = dynamic_cast<Binary*>(expression)){
            enter(binary);
        }else if(auto unary = dynamic_cast<Unary*>(expression)){
            enter(unary);
        }else if(auto identifier = dynamic_cast<Identifier*>(expression)){
            auto desc = currentScope->get(identifier->value);

            if(!desc->defined){
                ERROR(identifier->location);
            }

            identifier->superType = desc->superType;
        }else if(auto constant = dynamic_cast<Constant*>(expression)){
            constant->superType = new SimpleType(TYPE_CHAR);
        }else if(auto string = dynamic_cast<StringLiteral*>(expression)){
            string->superType = new SimpleType(TYPE_STRING);
        }else if(auto call = dynamic_cast<Call*>(expression)){
            for( auto expr : call->values ){
                enter(expr);
            }

            enter(call->target);
            call->superType = call->target->superType->apply(call);
        }else if(auto number = dynamic_cast<Number*>(expression)){
            number->type = TYPE_INT;
            number->superType = new SimpleType(TYPE_INT);
        }else if(auto select = dynamic_cast<Select*>(expression)){
            enter(select->target);
            enter(select->index);

            if( !select->index->superType->equals(IntType) ){
                ERROR(select->index->location);
            }

            select->superType = select->target->superType->apply(select);
        }else if(auto sizeOf = dynamic_cast<Sizeof*>(expression)){
            sizeOf->superType = new SimpleType(TYPE_INT);
        }else if(auto ifSmall = dynamic_cast<IfSmall*>(expression)){
            enter(ifSmall->condition);
            enter(ifSmall->left);
            enter(ifSmall->right);

            if(!ifSmall->left->superType->equals(ifSmall->right->superType)){
                ERROR(Location());
            }

            if(!ifSmall->condition->superType->equals(IntType)){
                ERROR(Location());
            }
        }else{
            ERROR(expression->location);
        }
    }

    void enter(Unary* unary){
        enter(unary->value);

        auto type = unary->value->superType;
        switch(unary->op->id){
            case STAR:
                if( type->isPointer() ){
                    auto dealloc = dynamic_cast<const DeallocateType*>(type);
                    unary->superType = dealloc->subType;
                    return;
                }
                break;
            case PLUS:
                if( type->isPointer() ){
                    unary->superType = type;
                    return;
                }
            case MINUS:
            case NOT:
            case SIZEOF:
                if( type->equals(IntType) ){
                    unary->superType = IntType;
                    return;
                }
                break;
            case AND:
                unary->superType = new DeallocateType(type);
                return;
        }

        ERROR(unary->op->location);
    }

    void enter(Binary* binary){
        if( binary->op->id == ASSIGN ){
            if( const auto* leftBin = dynamic_cast<const Binary *>(binary->left) ){
                if(leftBin->op->id != ARROW && leftBin->op->id != DOT){
                    fatal(binary->op);
                }
            }else if( const auto* leftUnary = dynamic_cast<const Unary *>(binary->left) ){
                if(leftUnary->op->id != STAR){
                    fatal(binary->op);
                }
            }
        }

        enter(binary->left);
        auto leftType = binary->left->superType;

        if(leftType == nullptr  ){
            ERROR(binary->op->location);
        }

        switch(binary->op->id) {
            case ARROW:
                if (auto deallocType = dynamic_cast<const DeallocateType *>(leftType)) {
                    if (auto superStruct = dynamic_cast<const SuperStructType *>(deallocType->subType)) {
                        auto identifier = (const Identifier *) binary->right;
                        binary->superType = superStruct->apply(identifier);
                        if (binary->superType != nullptr) {
                            return;
                        }
                    }
                }
                break;
            case DOT:
                if (auto superStruct = dynamic_cast<const SuperStructType *>(leftType)) {
                    auto identifier = (const Identifier *) binary->right;
                    binary->superType = superStruct->apply(identifier);
                    if (binary->superType != nullptr) {
                        return;
                    }
                }
                break;
        }

        enter(binary->right);
        auto rightType = binary->right->superType;

        if( rightType == nullptr ){
            ERROR(binary->op->location);
        }

        switch(binary->op->id){
            case STAR:
                if( leftType->equals(IntType) && rightType->equals(IntType) ){
                    binary->superType = new SimpleType(TYPE_INT);
                    return;
                }
                break;
            case PLUS:
                if( leftType->equals(IntType) && rightType->equals(IntType) ){
                    binary->superType = new SimpleType(TYPE_INT);
                    return;
                }else if(leftType->isPointer() && rightType->equals(IntType)){
                    binary->superType = leftType;
                    return;
                }else if(rightType->isPointer() && leftType->equals(IntType)){
                    binary->superType = rightType;
                    return;
                }
                break;
            case MINUS:
            case LESS:
            case LESS_EQUAL:
            case GREATER:
            case GREATER_EQUAL:
            case EQUAL:
            case NOT_EQUAL:
            case AND_AND:
            case OR_OR:
                if( leftType->equals(rightType) ){
                    binary->superType = new SimpleType(TYPE_INT);
                    return;
                }
                break;
            case LEFT_SHIFT:
            case RIGHT_SHIFT:
                if( leftType->equals(IntType) && rightType->equals(IntType) ){
                    binary->superType = new SimpleType(TYPE_INT);
                    return;
                }
                break;
            case ASSIGN:
                if( leftType->equals(rightType) ){
                    binary->superType = leftType;
                    return;
                }

                if( leftType->isPointer() && rightType->equals(IntType) ){
                    binary->superType = leftType;
                    return;
                }

                if( leftType->isPointer() && rightType->equals(StringType) ){
                    if(auto ref = dynamic_cast<const DeallocateType*>(leftType)){
                        if(ref->subType->equals(CharType)){
                            binary->superType = leftType;
                            return;
                        }
                    }
                }
                break;
        }

        ERROR(binary->op->location);
    }

    SuperType* enter(DirectDeclarator* directDeclarator, SuperType* simpleType){
        if(auto declarator = dynamic_cast<Declarator*>(directDeclarator)){

            for( int i = 0 ; i < declarator->pointer ; i++ ){
                simpleType = new DeallocateType(simpleType);
            }

            return enter(declarator->directDeclarator, simpleType);
        }else if(auto paramDecl = dynamic_cast<ParameterDirectDeclarator*>(directDeclarator)){
            auto methodType = new MethodType(simpleType);

            for( auto decl : paramDecl->parameterTypeList->declarations ){
                methodType->types.push_back(enter(decl));
            }

            if(auto identifier = dynamic_cast<Identifier*>(paramDecl->directDeclarator)){
                methodType->identifier = identifier;
            }else if(paramDecl->directDeclarator != nullptr){
                enter(paramDecl->directDeclarator, methodType);
            }

            return methodType;
        }else if(auto identifier = dynamic_cast<Identifier*>(directDeclarator)){
            simpleType->identifier = identifier;
            return simpleType;
        }

        return simpleType;
    }

    const SuperType* enter0(Declaration *declaration) {
        if(declaration != nullptr){
            auto type = enter(declaration);
            if(type != nullptr && type->identifier != nullptr && !currentScope->set(type->identifier->value, type)){
                currentScope->set(type->identifier->value, type);
                ERROR(type->identifier->location);
            }

            return type;
        }

        return nullptr;
    }

    SuperType* enter(Declaration *declaration) {
        SuperType* superType = nullptr;
        if(auto structType = dynamic_cast<StructType*>(declaration->type)){
            auto superStruct = new SuperStructType();

            if(structType->declarations.empty()){
                superType = currentScope->getStruct(structType->name)->superType;
            }else{
                for( auto decel : structType->declarations ){
                    auto structInner = enter(decel);

                    superStruct->types.push_back(structInner);

                    auto identifier = structInner->identifier;
                    if(identifier != nullptr){
                        superStruct->map[identifier->value] = superStruct->types.size() - 1;
                    }
                }

                currentScope->setStruct(structType->name, superStruct);
                //currentScope->structs[structType->name] = superStruct;
                superType = superStruct;
            }
        }else{
            switch (declaration->type->type) {
                case TYPE_BOOL: superType = new SimpleType(TYPE_BOOL); break;
                case TYPE_VOID: superType = new SimpleType(TYPE_VOID); break;
                case TYPE_CHAR: superType = new SimpleType(TYPE_CHAR); break;
                case TYPE_INT: superType = new SimpleType(TYPE_INT); break;
                case TYPE_STRING: superType = new SimpleType(TYPE_STRING); break;
            }
        }

        if(superType == nullptr){
            ERROR(declaration->location);
        }

        return enter(declaration->declarator, superType);
    }
};