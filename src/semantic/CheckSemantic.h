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
extern SuperType* const StringType = new PointerType(CharType);

#define ERROR(LOCATION) (throw SemanticException(LOCATION, __LINE__))
#define ERROR_MSG(LOCATION, MSG) (throw SemanticException(LOCATION,MSG, __LINE__))

template<class T>
class Descriptor{
public:
    T* superType = nullptr;
    bool implementation = false;
    bool defined = false;

    Descriptor(){

    }

    Descriptor(const Descriptor* other) : superType(other->superType), implementation(other->implementation), defined(other->defined){

    }

    Descriptor(SuperType* superType) : superType(superType){

    }
};

class Scope{
public:
    std::unordered_map<const std::string*, Descriptor<SuperType>> types;
    std::unordered_map<const std::string*, Descriptor<SuperStructType>> structs;
    std::unordered_map<const std::string*, int> labels;
    Scope* parent = nullptr;
    const SuperType* returnType = nullptr;

    Scope(){

    }

    bool isLabel(const std::string* label){
        return labels.find(label) != labels.end();
    }

    void setLabel(const std::string* label){
        labels[label] = 0;
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
            if(!desc->superType->equals(type)){
                return false;
            }
        }else{
            desc->superType = type;
            desc->defined = true;
        }

        return true;
    }

    bool set(const std::string* str, SuperType* type, bool hasImplementation){
        auto desc = &types[str];

        bool isMethod = instanceof<MethodType>(type);

        if( desc->defined ){
            if( !isMethod ){
                return false;
            }else if(!desc->superType->equals(type)){
                return false;
            }
        }else{
            desc->superType = type;
            desc->defined = true;
        }

        if(hasImplementation){
            if(desc->implementation){
                return false;
            }else{
                desc->implementation = true;
            }
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
    Scope mainScope;
    Scope* currentScope = &mainScope;
    Scope* methodScope = nullptr;

    void checkType( const Expression* element, const SuperType* superType ){
        if( element->superType == nullptr && !element->superType->equals(superType) ){
            ERROR(element->location);
        }
    }

    void check(Unit *element) {
        for (auto child : element->children) {
            if (auto *declaration = dynamic_cast<Declaration *>(child)) {
                enter0(declaration);
            } else if (auto *method = dynamic_cast<Method *>(child)) {
                auto methodType = static_cast<MethodType*>(enter(method->declaration));

                auto inner = new Scope();

                if(methodType->identifier != nullptr && !currentScope->set(methodType->identifier->value, methodType,true)){
                    ERROR(methodType->identifier->location);
                }

                if(methodType->types.size() != 1 || !methodType->types[0]->equals(VoidType)){
                    for(  int i = 0 ; i < methodType->types.size(); i++  ){
                        auto paramType = methodType->types[i];
                        auto identifier = paramType->identifier;

                        if(identifier == nullptr){
                            if(i < methodType->locations.size()){
                                ERROR(methodType->locations[i]);
                            }
                            ERROR(method->declaration->location);
                        }else{
                            if(!inner->set(identifier->value, paramType, false)){
                                ERROR(identifier->location);
                            }
                        }
                    }
                }

                inner->parent = currentScope;
                inner->returnType = methodType->subType;
                methodScope = inner;
                findLabels(method->body);
                enter(method->body, inner);
            }
        }
    }

    void findLabels(Statement* statement){
        if(statement == nullptr){
            return;
        }else if(auto labeledStatement = dynamic_cast<LabeledStatement*>(statement)){
            if(methodScope->isLabel(labeledStatement->name)){
                ERROR(labeledStatement->location);
            }
            methodScope->setLabel(labeledStatement->name);
            findLabels(labeledStatement->statement);
        }else if(auto ifStatement = dynamic_cast<If*>(statement)){
            findLabels(ifStatement->trueBranch);
            findLabels(ifStatement->falseBranch);
        }else if(auto whileStatement = dynamic_cast<While*>(statement)){
            findLabels(whileStatement->body);
        }else if(auto block = dynamic_cast<Block*>(statement)){
            for(auto &child : block->children){
                findLabels(child);
            }
        }
    }

    void enter(Block* block, Scope* scope){
        if( block != nullptr ){
            Scope* carry = currentScope;
            if(scope == nullptr){
                currentScope = new Scope();
            }else{
                currentScope = scope;
            }
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
            enter(labeledStatement->statement);
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
            enter(block, nullptr);
        }else if(auto declaration = dynamic_cast<Declaration*>(statement)){
            enter0(declaration);
        }else if(auto expression = dynamic_cast<Expression*>(statement)){
            enter(expression);
        }else{
            ERROR(statement->location);
        }
    }



    void checkCondition(Expression* condition, Location& location){
        enter(condition);
        if( instanceof<SuperStructType>(condition->superType) ){
            ERROR(location);
        }
    }

    void enter(If* ifStatement){
        enter(ifStatement->condition);
        enter(ifStatement->trueBranch);
        enter(ifStatement->falseBranch);

        if( instanceof<SuperStructType>(ifStatement->condition->superType) ){
            ERROR(ifStatement->location);
        }

        checkCondition(ifStatement->condition, ifStatement->location);
    }

    void enter(While* whileStatement){
        checkCondition(whileStatement->condition, whileStatement->location);
        enter(whileStatement->body);
    }

    void enter(GoTo* gotoStatement){
        if(!methodScope->isLabel(gotoStatement->name)){
            ERROR(gotoStatement->location);
        }
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

        if(!isAssignable(currentScope->getReturnType(), returnType)){
            ERROR(loc->location);
        }
    }

    bool isAssignable(const SuperType* target, const SuperType* source)
    {
        return target->equals(source) || (target->asPointerType() && source->equals(IntType));
    }

    void enter(Continue* continueStatement){

    }

    void enter(Break* breakStatement){

    }

    void enter(Expression* expression){
        enter0(expression);

        if( expression->superType == nullptr ){
            ERROR(expression->location);
        }
    }

    void enter0(Expression* expression){
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
            constant->superType = new ProxyType(CharType, false);
        }else if(auto string = dynamic_cast<StringLiteral*>(expression)){
            string->superType = new ProxyType(StringType, false);
        }else if(auto call = dynamic_cast<Call*>(expression)){
            for( auto expr : call->values ){
                enter(expr);
            }

            enter(call->target);


            if( auto methodType = call->target->superType->asMethodType() ){

                if( methodType->types.size() < call->values.size() ){
                    ERROR(call->locations[methodType->types.size()]);
                }else if(methodType->types.size() > call->values.size()){
                    ERROR(call->locations[call->locations.size() - 1]);
                }

                int min = std::min(methodType->types.size() , call->values.size());

                for( int i = 0 ; i < min ; i++ ){
                    if( !methodType->types[i]->equals(call->values[i]->superType) ){
                        ERROR(call->locations[i]);
                    }
                }

                call->superType = new ProxyType(methodType->subType, false);


                if(call->superType == nullptr){
                    ERROR(call->location);
                }
                return;
            }


            ERROR(call->target->location);
        }else if(auto number = dynamic_cast<Number*>(expression)){
            number->superType = IntType;
        }else if(auto select = dynamic_cast<Select*>(expression)){
            enter(select->target);
            enter(select->index);

            if( !select->index->superType->asSimpleType() && !select->index->superType->asSimpleType() ){
                ERROR(select->index->location);
            }

            auto type = select->target->superType;

            select->superType = type->select();
        }else if(auto sizeOf = dynamic_cast<Sizeof*>(expression)){
            sizeOf->superType = IntType;
        }else if(auto ifSmall = dynamic_cast<IfSmall*>(expression)){
            enter(ifSmall->condition);
            enter(ifSmall->left);
            enter(ifSmall->right);

            checkType(ifSmall->condition, IntType);
            checkType(ifSmall->left, ifSmall->right->superType);

            ifSmall->superType = ifSmall->left->superType;
        }else{
            ERROR(expression->location);
        }
    }

    void enter(Unary* unary){
        enter(unary->value);

        auto type = unary->value->superType;
        switch(unary->op->id){
            case STAR:
                if( auto dealloc = type->asPointerType() ){
                    unary->superType = new ProxyType(dealloc->subType, true);
                    return;
                }
                break;
            case PLUS:
                if( type->asPointerType() ){
                    unary->superType = new ProxyType(type, false);
                    return;
                }
            case MINUS:
            case NOT:
            case SIZEOF:
                unary->superType = IntType;
                return;
            case AND:
                unary->superType = new PointerType(type);
                return;
        }

        ERROR(unary->op->location);
    }

    void enter(Binary* binary){
        /*if( binary->op->id == ASSIGN ){
            if( const auto* leftBin = dynamic_cast<const Binary *>(binary->left) ){
                if(leftBin->op->id != ARROW && leftBin->op->id != DOT){
                    ERROR(binary->op->location);
                }
            }else if( const auto* leftUnary = dynamic_cast<const Unary *>(binary->left) ){
                if(leftUnary->op->id != STAR){
                    ERROR(binary->op->location);
                }
            }
        }*/

        enter(binary->left);
        auto leftType = binary->left->superType;

        if(leftType == nullptr){
            ERROR(binary->op->location);
        }

        switch(binary->op->id) {
            case ARROW:
                if (auto deallocType = leftType->asPointerType()) {
                    if (auto superStruct = deallocType->subType->asSuperStructType()) {
                        auto identifier = (const Identifier *) binary->right;
                        binary->superType = superStruct->member(identifier);
                        if (binary->superType != nullptr) {
                            return;
                        }
                    }
                }
                break;
            case DOT:
                if (auto superStruct = leftType->asSuperStructType()) {
                    auto identifier = (const Identifier *) binary->right;
                    binary->superType = superStruct->member(identifier);
                    if (binary->superType != nullptr) {
                        return;
                    }
                }
                ERROR(binary->op->location);
        }

        enter(binary->right);
        auto rightType = binary->right->superType;

        if( rightType == nullptr ){
            ERROR(binary->op->location);
        }

        switch(binary->op->id){
            case STAR:
                if( leftType->equals(IntType) && rightType->equals(IntType) ){
                    binary->superType = new SimpleType(TYPE_INT, false);
                    return;
                }
                break;
            case PLUS:
                if( leftType->equals(IntType) && rightType->equals(IntType) ){
                    binary->superType = new SimpleType(TYPE_INT, false);
                    return;
                }else if(leftType->asPointerType() && rightType->equals(IntType)){
                    binary->superType = new ProxyType(leftType, false);
                    return;
                }else if(rightType->asPointerType() && leftType->equals(IntType)){
                    binary->superType = new ProxyType(rightType, false);
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
                    binary->superType = new SimpleType(TYPE_INT, false);
                    return;
                }
                break;
            case LEFT_SHIFT:
            case RIGHT_SHIFT:
                if( leftType->equals(IntType) && rightType->equals(IntType) ){
                    binary->superType = new SimpleType(TYPE_INT, false);
                    return;
                }
                break;
            case ASSIGN:

                if(!leftType->assignable){
                    ERROR_MSG(binary->op->location, binary->left->toString() + " not assignable");
                }

                if(isAssignable(leftType, rightType)){
                    binary->superType = rightType;
                    return;
                }

                break;
        }

        ERROR(binary->op->location);
    }

    SuperType* enter(DirectDeclarator* directDeclarator, SuperType* simpleType){
        if(auto declarator = dynamic_cast<Declarator*>(directDeclarator)){

            for( int i = 0 ; i < declarator->pointer ; i++ ){
                simpleType = new PointerType(simpleType, true);
            }

            return enter(declarator->directDeclarator, simpleType);
        }else if(auto paramDecl = dynamic_cast<ParameterDirectDeclarator*>(directDeclarator)){
            auto methodType = new MethodType(simpleType);

            auto decls = paramDecl->parameterTypeList->declarations;

            if(decls.size() == 1){
                auto decl = decls[0];

                auto type = enter(decl);

                if(!type->equals(VoidType)){
                    methodType->types.push_back(type);
                }
            }else{
                for( auto decl : decls ){

                    auto type = enter(decl);

                    if(VoidType->equals(type)){
                        ERROR(decl->location);
                    }
                    methodType->types.push_back(type);
                }
            }

            methodType->locations = paramDecl->parameterTypeList->locations;
            SuperType* finalType = methodType;

            if(paramDecl->directDeclarator != nullptr){
                finalType = enter(paramDecl->directDeclarator, methodType);
            }

            /*if(auto identifier = dynamic_cast<Identifier*>(paramDecl->directDeclarator)){
                if(VoidType->equals(finalType)){
                    ERROR(identifier->location);
                }

                finalType->identifier = identifier;
            }*/

            return enter(paramDecl->directDeclarator, finalType);
        }else if(auto identifier = dynamic_cast<Identifier*>(directDeclarator)){
            if(VoidType->equals(simpleType)){
                ERROR(identifier->location);
            }

            simpleType->identifier = identifier;
            return simpleType;
        }

        return simpleType;
    }

    const SuperType* enter0(Declaration *declaration) {
        if(declaration != nullptr){
            auto type = enter(declaration);
            if(type != nullptr ){
                if( instanceof<SimpleType>(type) && type->identifier == nullptr){
                    ERROR(declaration->location);
                }

                if(type->identifier != nullptr && !currentScope->set(type->identifier->value, type, false)){
                    ERROR(type->identifier->location);
                }
            }

            return type;
        }

        return nullptr;
    }

    SuperType* enter(Declaration *declaration) {
        SuperType* superType = nullptr;
        if(auto structType = dynamic_cast<StructType*>(declaration->type)){
            auto superStruct = new SuperStructType(true);

            auto desc = currentScope->getStruct(structType->name);

            if(structType->name != nullptr && desc != nullptr && desc->defined){
                if(!structType->declarations.empty()){
                    ERROR(structType->location);
                }

                superType = desc->superType;
            }else{
                for( auto decel : structType->declarations ){

                    auto savedScope = currentScope;
                    currentScope = new Scope();
                    auto structInner = enter0(decel);
                    delete currentScope;
                    currentScope = savedScope;

                    if(structInner == nullptr){
                        ERROR(structType->location);
                    }

                    superStruct->types.push_back(structInner);

                    auto identifier = structInner->identifier;
                    if(identifier != nullptr){
                        if(superStruct->map.find(identifier->value) != superStruct->map.end()){
                            ERROR(identifier->location);
                        }

                        superStruct->map[identifier->value] = superStruct->types.size() - 1;
                    }
                }

                //hello
                //set(type->identifier->value, type, false)

                currentScope->setStruct(structType->name, superStruct);

                //currentScope->structs[structType->name] = superStruct;
                superType = superStruct;
            }
        }else{
            switch (declaration->type->type) {
                case TYPE_BOOL: superType = new SimpleType(TYPE_BOOL, true); break;
                case TYPE_VOID: superType = new SimpleType(TYPE_VOID, true); break;
                case TYPE_CHAR: superType = new SimpleType(TYPE_CHAR, true); break;
                case TYPE_INT: superType = new SimpleType(TYPE_INT, true); break;
            }
        }

        if(superType == nullptr){
            ERROR(declaration->location);
        }

        return enter(declaration->declarator, superType);
    }
};
