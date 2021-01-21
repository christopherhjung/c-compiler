//
// Created by Christopher Jung on 21.12.20.
//

#pragma once


#include "../lexer/Location.h"
#include "SemanticException.h"

#include "../elements/Elements.h"
#include "../types/Types.h"
#include "../types/TypeDefines.h"

#include "Scope.h"


class Semantic {
public:
    Scope mainScope;
    Scope *currentScope = &mainScope;
    Scope *methodScope = nullptr;

    void checkType(const Expression *element, const SuperType *superType);

    void check(Unit *element);

    void findLabels(Statement *statement);

    void enter(Block *block, Scope *scope);

    void enter(Statement *statement);

    void checkCondition(Expression *condition, Location &location);

    void enter(If *ifStatement);

    void enter(While *whileStatement);

    void enter(GoTo *gotoStatement);

    void enter(Return *returnStatement);

    bool isAssignable(const SuperType *target, const SuperType *source);

    void enter(Expression *expression);

    void enter0(Expression *expression);

    void enter(Unary *unary);

    void enter(Binary *binary);

    SuperType *enter(DirectDeclarator *directDeclarator, SuperType *simpleType);

    const SuperType *enter0(Declaration *declaration);

    SuperType *enter(Declaration *declaration);
};