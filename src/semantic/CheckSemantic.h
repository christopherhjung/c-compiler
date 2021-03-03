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
    Scope *mainScope = new Scope();
    Scope *currentScope = mainScope;
    Scope *methodScope = nullptr;

    void checkUnit(Unit *element);

    void findLabels(Statement *statement);

    void checkBlock(Block *block, Scope *scope);

    void checkStatement(Statement *statement);

    void checkCondition(Expression *condition, Location &location);

    void checkIf(If *ifStatement);

    void checkWhile(While *whileStatement);

    void checkGoTo(GoTo *gotoStatement);

    void checkReturn(Return *returnStatement);

    bool isAssignable(const SemanticType *target, const SemanticType *source);

    void checkExpression(Expression *expression);

    void checkNullableExpression(Expression *expression);

    void checkUnary(Unary *unary);

    void checkBinary(Binary *binary);
    SemanticType *checkDeclarator(Declarator *declarator, Type* type, Location* location);
    SemanticType *checkDirectDeclarator(DirectDeclarator *directDeclarator, SemanticType *simpleType);

    const SemanticType *checkDeclarationWithScopeCheck(Declaration *declaration);

    SemanticType *checkDeclaration(Declaration *declaration);
};