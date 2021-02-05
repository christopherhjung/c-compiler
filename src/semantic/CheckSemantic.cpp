//
// Created by Christopher Jung on 21.12.20.
//

#include "CheckSemantic.h"
#include "../types/Types.h"


void Semantic::checkType(const Expression *element, const SemanticType *semanticType) {
    if (element->semanticType == nullptr || !element->semanticType->equals(semanticType)) {
        ERROR(element->location);
    }
}

void Semantic::check(Unit *element) {
    element->scope = mainScope;
    for (auto child : element->children) {
        if (auto *declaration = dynamic_cast<Declaration *>(child)) {
            enter0(declaration);
        } else if (auto *method = dynamic_cast<Method *>(child)) {
            auto methodType = const_cast<MethodType *>(enter0(method->declaration)->asMethodType());

            auto inner = new Scope();
            auto name = methodType->identifier->value;
            if (methodType->identifier != nullptr &&
                !currentScope->set(name, methodType, true)) {

                const_cast<Identifier*>(methodType->identifier)->anchor = currentScope->get(name)->anchor;
                ERROR(methodType->identifier->location);
            }

            if (methodType->types.size() != 1 || !methodType->types[0]->equals(VoidType)) {
                for (unsigned long i = 0; i < methodType->types.size(); i++) {
                    auto paramType = methodType->types[i];
                    auto identifier = paramType->identifier;

                    if (identifier == nullptr) {
                        if (i < methodType->locations.size()) {
                            ERROR(methodType->locations[i]);
                        }
                        ERROR(method->declaration->location);
                    } else {
                        if (!inner->set(identifier->value, paramType, false)) {
                            ERROR(identifier->location);
                        }else{
                            const_cast<Identifier*>(identifier)->anchor = inner->get(identifier->value)->anchor;
                        }
                    }
                }
            }

            inner->parent = currentScope;
            inner->returnType = methodType->subType;
            methodScope = inner;
            findLabels(method->body);
            enter(method->body, inner);
            methodScope = nullptr;
        }
    }
}

void Semantic::findLabels(Statement *statement) {
    if (statement == nullptr) {
        return;
    } else if (auto labeledStatement = dynamic_cast<LabeledStatement *>(statement)) {
        if (methodScope->isLabel(labeledStatement->name)) {
            ERROR(labeledStatement->location);
        }
        methodScope->setLabel(labeledStatement->name);
        findLabels(labeledStatement->statement);
    } else if (auto ifStatement = dynamic_cast<If *>(statement)) {
        findLabels(ifStatement->trueBranch);
        findLabels(ifStatement->falseBranch);
    } else if (auto whileStatement = dynamic_cast<While *>(statement)) {
        findLabels(whileStatement->body);
    } else if (auto block = dynamic_cast<Block *>(statement)) {
        for (auto &child : block->children) {
            findLabels(child);
        }
    }
}

void Semantic::enter(Block *block, Scope *scope) {
    if (block != nullptr) {
        Scope *carry = currentScope;
        if (scope == nullptr) {
            currentScope = new Scope();
        } else {
            currentScope = scope;
        }

        block->scope = currentScope;

        currentScope->parent = carry;
        for (auto statement : block->children) {
            enter(statement);
        }

        currentScope = carry;
    }
}

void Semantic::enter(Statement *statement) {
    if (statement == nullptr) {
        return;
    } else if (auto labeledStatement = dynamic_cast<LabeledStatement *>(statement)) {
        enter(labeledStatement->statement);
    } else if (auto ifStatement = dynamic_cast<If *>(statement)) {
        enter(ifStatement);
    } else if (auto whileStatement = dynamic_cast<While *>(statement)) {
        enter(whileStatement);
    } else if (auto returnStatement = dynamic_cast<Return *>(statement)) {
        enter(returnStatement);
    } else if (auto gotoStatement = dynamic_cast<GoTo *>(statement)) {
        enter(gotoStatement);
    }/*else if(auto continueStatement = dynamic_cast<Continue*>(statement)){
            enter(continueStatement);
        }else if(auto breakStatement = dynamic_cast<Break*>(statement)){
            enter(breakStatement);
        }*/else if (auto block = dynamic_cast<Block *>(statement)) {
        enter(block, nullptr);
    } else if (auto declaration = dynamic_cast<Declaration *>(statement)) {
        enter0(declaration);
    } else if (auto expression = dynamic_cast<Expression *>(statement)) {
        enter(expression);
    }/*else{
            ERROR(statement->location);
        }*/
}

void Semantic::checkCondition(Expression *condition, Location &location) {
    enter(condition);
    if (condition->semanticType->asSuperStructType()) {
        ERROR(location);
    }
}

void Semantic::enter(If *ifStatement) {
    enter(ifStatement->condition);
    enter(ifStatement->trueBranch);
    enter(ifStatement->falseBranch);

    checkCondition(ifStatement->condition, ifStatement->location);
}

void Semantic::enter(While *whileStatement) {
    checkCondition(whileStatement->condition, whileStatement->location);
    enter(whileStatement->body);
}

void Semantic::enter(GoTo *gotoStatement) {
    if (!methodScope->isLabel(gotoStatement->name)) {
        ERROR(gotoStatement->location);
    }
}

void Semantic::enter(Return *returnStatement) {
    Element *loc;
    if (returnStatement->value == nullptr) {
        loc = returnStatement;
    } else {
        enter(returnStatement->value);
        loc = returnStatement->value;
    }

    returnStatement->semanticType = methodScope->returnType;
    if (!isAssignable(currentScope->getReturnType(), returnStatement->value)) {
        ERROR(loc->location);
    }
}

bool Semantic::isAssignable(const SemanticType *target, Expression *sourceStatement) {
    const SemanticType* source;
    if(sourceStatement == nullptr){
        source = VoidType;
    }else{
        source = sourceStatement->semanticType;
    }

    bool targetIsNumeric = target->equals(IntType) || target->equals(CharType);
    bool sourceIsNumeric = source->equals(IntType) || source->equals(CharType);

    bool assignable = target->equals(source)
           || (targetIsNumeric && sourceIsNumeric)
           || (target->asPointerType() && sourceIsNumeric)
           || (target->equals(VoidPointerType) && source->asPointerType())
           || (source->equals(VoidPointerType) && target->asPointerType());

    if(!assignable && sourceStatement != nullptr){
        if(auto targetPointer = target->asPointerType()){
            if(auto sourceMethod = source->asMethodType()){
                if(auto targetMethod = targetPointer->subType->asMethodType()){
                    assignable = sourceMethod->equals(targetMethod);
                }
            }
        }
    }

    return assignable;
}

/*
void enter(Continue*){

}

void enter(Break*){
}*/

void Semantic::enter(Expression *expression) {
    enter0(expression);

    if (expression->semanticType == nullptr) {
        ERROR(expression->location);
    }
}

void Semantic::enter0(Expression *expression) {
    if (expression == nullptr) {
        return;
    } else if (auto binary = dynamic_cast<Binary *>(expression)) {
        enter(binary);
    } else if (auto unary = dynamic_cast<Unary *>(expression)) {
        enter(unary);
    } else if (auto identifier = dynamic_cast<Identifier *>(expression)) {
        auto desc = currentScope->get(identifier->value);

        if (!desc->defined) {
            ERROR(identifier->location);
        }

        identifier->anchor = desc->anchor;
        identifier->semanticType = desc->semanticType;
    } else if (auto constant = dynamic_cast<Constant *>(expression)) {
        constant->semanticType = new SimpleType(TYPE_CHAR, false);
    } else if (auto string = dynamic_cast<StringLiteral *>(expression)) {
        string->semanticType = new ProxyType(CharPointerType, false);
    } else if (auto call = dynamic_cast<Call *>(expression)) {
        for (auto expr : call->values) {
            enter(expr);
        }

        enter(call->target);
        auto methodType = call->target->semanticType->asMethodType();
        if(!methodType){
            auto pointerType = call->target->semanticType->asPointerType();
            if(pointerType){
                methodType = pointerType->subType->asMethodType();
            }
        }

        if (methodType) {
            if (methodType->types.size() < call->values.size()) {
                ERROR(call->locations[methodType->types.size()]);
            } else if (methodType->types.size() > call->values.size()) {
                if (!(methodType->types.size() == 1 && methodType->types[0]->equals(VoidType))) {
                    ERROR(call->locations[call->locations.size() - 1]);
                }
            } else {
                int min = std::min(methodType->types.size(), call->values.size());

                for (int i = 0; i < min; i++) {
                    if(!isAssignable(methodType->types[i], call->values[i])){
                        ERROR(call->locations[i]);
                    }
                }
            }

            if (methodType->subType == nullptr) {
                ERROR(call->location);
            }

            call->semanticType = new ProxyType(methodType->subType, false);
            return;
        }

        ERROR(call->target->location);
    } else if (auto number = dynamic_cast<Number *>(expression)) {
        number->semanticType = IntType;
    } else if (auto select = dynamic_cast<Select *>(expression)) {
        enter(select->target);
        enter(select->index);

        auto rightType = select->index->semanticType;
        auto leftType = select->target->semanticType;

        if (auto pointer = leftType->asPointerType()) {
            if (!IntType->equals(rightType)) {
                ERROR(select->index->location);
            }

            select->semanticType = pointer->subType;
        } else if (auto pointer = rightType->asPointerType()) {
            if (!IntType->equals(leftType)) {
                ERROR(select->target->location);
            }

            select->semanticType = pointer->subType;
        }

    } else if (auto sizeOf = dynamic_cast<Sizeof *>(expression)) {
        sizeOf->inner = enter(sizeOf->declarator, sizeOf->type, &sizeOf->location);
        sizeOf->semanticType = IntType;
    } else if (auto ifSmall = dynamic_cast<IfSmall *>(expression)) {
        checkCondition(ifSmall->condition, ifSmall->condition->location);
        enter(ifSmall->left);
        enter(ifSmall->right);

        checkType(ifSmall->left, ifSmall->right->semanticType);

        ifSmall->semanticType = ifSmall->left->semanticType;
    } else {
        ERROR(expression->location);
    }
}

void Semantic::enter(Unary *unary) {
    enter(unary->value);

    auto type = unary->value->semanticType;
    switch (unary->op->id) {
        case STAR:
            if (auto dealloc = type->asPointerType()) {
                if(dealloc->asMethodType()){
                    unary->semanticType = new ProxyType(dealloc, true);
                }else{
                    unary->semanticType = new ProxyType(dealloc->subType, true);
                }
                return;
            }else if(type->asMethodType()){
                unary->semanticType = new ProxyType(type, false);
                return;
            }
            break;
        case PLUS:
            if (type->asPointerType()) {
                unary->semanticType = new ProxyType(type, false);
                return;
            }
        case MINUS:
        case NOT:
        case SIZEOF:
            unary->semanticType = IntType;
            return;
        case AND:
            unary->semanticType = new PointerType(type);
            return;
    }

    ERROR(unary->op->location);
}

void Semantic::enter(Binary *binary) {

    enter(binary->left);
    auto leftType = binary->left->semanticType;

    if (leftType == nullptr) {
        ERROR(binary->op->location);
    }

    switch (binary->op->id) {
        case ARROW:
            if (auto deallocType = leftType->asPointerType()) {
                if (auto superStruct = deallocType->subType->asSuperStructType()) {
                    if (auto identifier = dynamic_cast<Identifier *>(binary->right)) {
                        binary->semanticType = superStruct->member(identifier);
                        if (binary->semanticType != nullptr) {
                            return;
                        }
                    }
                }
            }
            ERROR(binary->op->location);
            break;
        case DOT:
            if (auto superStruct = leftType->asSuperStructType()) {
                if (auto identifier = dynamic_cast<Identifier *>(binary->right)) {
                    binary->semanticType = superStruct->member(identifier);
                    if (binary->semanticType != nullptr) {
                        return;
                    }
                }
            }
            ERROR(binary->op->location);
            break;
    }

    enter(binary->right);
    auto rightType = binary->right->semanticType;

    if (rightType == nullptr) {
        ERROR(binary->op->location);
    }

    bool leftIsInteger = IntType->equals(leftType);
    bool rightIsInteger = IntType->equals(rightType);

    bool leftIsPointer = IntType->equals(leftType);
    bool rightIsPointer = IntType->equals(rightType);

    bool leftIsNumeric = leftIsInteger || leftType->equals(CharType);
    bool rightIsNumeric = rightIsInteger || rightType->equals(CharType);

    bool leftIsStruct = leftType->asSuperStructType();
    bool rightIsStruct = rightType->asSuperStructType();

    bool leftIsComparable = !(leftIsStruct || VoidType->equals(leftType));
    bool rightIsComparable = !(rightIsStruct || VoidType->equals(rightType));


    switch (binary->op->id) {
        case STAR:
            if (leftIsNumeric && rightIsNumeric) {
                binary->semanticType = new SimpleType(TYPE_INT, false);
                return;
            }
            break;
        case PLUS:
            if (leftIsNumeric && rightIsNumeric) {
                binary->semanticType = new SimpleType(TYPE_INT, false);
                return;
            } else if (leftType->asPointerType() && rightIsInteger) {
                binary->opInfo = 1;
                binary->semanticType = new ProxyType(leftType, false);
                return;
            } else if (rightType->asPointerType() && leftIsInteger) {
                binary->opInfo = 2;
                binary->semanticType = new ProxyType(rightType, false);
                return;
            }
            break;
        case MINUS:
            if (leftIsNumeric && rightIsNumeric) {
                binary->semanticType = new SimpleType(TYPE_INT, false);
                return;
            } else if (leftType->asPointerType() && rightIsInteger) {
                binary->opInfo = 1;
                binary->semanticType = new ProxyType(leftType, false);
                return;
            } else if (leftType->asPointerType() && rightType->asPointerType() && leftType->equals(rightType)) {
                binary->opInfo = 2;
                binary->semanticType = new SimpleType(TYPE_INT, false);
                return;
            }
            break;
        case EQUAL:
        case NOT_EQUAL:
            if (leftIsNumeric && rightIsNumeric) {
                binary->semanticType = new SimpleType(TYPE_INT, false);
                return;
            } else if (leftType->asPointerType() && rightIsInteger) {
                binary->semanticType = new SimpleType(TYPE_INT, false);
                return;
            } else if (rightType->asPointerType() && leftIsInteger) {
                binary->semanticType = new SimpleType(TYPE_INT, false);
                return;
            } else if (rightType->equals(VoidPointerType) && leftType->asPointerType()) {
                binary->semanticType = new SimpleType(TYPE_INT, false);
                return;
            } else if (leftType->equals(VoidPointerType) && rightType->asPointerType()) {
                binary->semanticType = new SimpleType(TYPE_INT, false);
                return;
            } else if (leftType->equals(rightType)) {
                binary->semanticType = new SimpleType(TYPE_INT, false);
                return;
            }
            break;
        case LESS:
        case LESS_EQUAL:
        case GREATER:
        case GREATER_EQUAL:
            if (leftIsNumeric && rightIsNumeric) {
                binary->semanticType = new SimpleType(TYPE_INT, false);
                return;
            } else if (leftIsPointer && rightIsPointer) {
                binary->semanticType = new SimpleType(TYPE_INT, false);
                return;
            }
            break;

        case AND_AND:
        case OR_OR:
            if (leftIsComparable && rightIsComparable) {
                binary->semanticType = new SimpleType(TYPE_INT, false);
                return;
            }
            break;
        case LEFT_SHIFT:
        case RIGHT_SHIFT:
            if (leftIsInteger && rightIsInteger) {
                binary->semanticType = new SimpleType(TYPE_INT, false);
                return;
            }
            break;
        case ASSIGN:
            if (!leftType->isAssignable()) {
                ERROR_MSG(binary->op->location, binary->left->toString() + " not assignable");
            }

            if (isAssignable(leftType, binary->right)) {
                binary->semanticType = leftType;
                return;
            }

            ERROR(binary->op->location);

            break;
    }

    ERROR(binary->op->location);
}

SemanticType *Semantic::enter(DirectDeclarator *directDeclarator, SemanticType *simpleType) {
    if (auto declarator = dynamic_cast<Declarator *>(directDeclarator)) {

        for (int i = 0; i < declarator->pointer; i++) {
            simpleType = new PointerType(simpleType, true);
        }

        return enter(declarator->directDeclarator, simpleType);
    } else if (auto paramDecl = dynamic_cast<ParameterDirectDeclarator *>(directDeclarator)) {
        auto methodType = new MethodType(simpleType);

        auto decls = paramDecl->parameterTypeList->declarations;

        if (decls.size() == 1) {
            methodType->types.push_back(enter(decls[0]));
        } else {
            for (auto decl : decls) {
                auto type = enter(decl);
                if (VoidType->equals(type)) {
                    ERROR(decl->location);
                }
                methodType->types.push_back(type);
            }
        }

        methodType->locations = paramDecl->parameterTypeList->locations;

        return enter(paramDecl->directDeclarator, methodType);
    } else if (auto identifier = dynamic_cast<Identifier *>(directDeclarator)) {
        if (VoidType->equals(simpleType)) {
            ERROR(identifier->location);
        }

        simpleType->identifier = identifier;
        identifier->semanticType = simpleType;
        return simpleType;
    }

    return simpleType;
}

const SemanticType *Semantic::enter0(Declaration *declaration) {
    if (declaration != nullptr) {
        auto type = enter(declaration);
        declaration->semanticType = type;
        if (type != nullptr) {
            if (type->asSimpleType() && type->identifier == nullptr) {
                ERROR(declaration->location);
            }

            if (type->identifier != nullptr) {
                if(!currentScope->set(type->identifier->value, type, false)){
                    ERROR(type->identifier->location);
                }

                const_cast<Identifier*>(type->identifier)->anchor = currentScope->get(type->identifier->value)->anchor;
            }
        }

        return type;
    }

    return nullptr;
}


SemanticType *Semantic::enter(Declaration *declaration) {
    auto leftType = enter(declaration->declarator, declaration->type, &declaration->location);
    if(declaration->initializer){
        enter(declaration->initializer);

        auto rightType = declaration->initializer;

        if (!isAssignable(leftType, rightType)) {
            ERROR(declaration->location);
        }
    }
    return leftType;
}

SemanticType *Semantic::enter(Declarator *declarator, Type* type, Location* location) {
    SemanticType *semanticType = nullptr;
    if (auto structType = dynamic_cast<StructType *>(type)) {
        auto superStruct = new SemanticStructType(true);

        if (structType->name != nullptr && structType->declarations.empty()) {
            if(methodScope == nullptr){
                semanticType = new PendingSuperStructType(structType->name, currentScope);
            }else{
                auto desc = currentScope->getStruct(structType->name);

                if (desc == nullptr) {
                    ERROR(structType->location);
                }

                semanticType = new ProxyType(desc->semanticType, true);
            }
        } else {
            if (structType->name != nullptr && currentScope->structDefinedInScope(structType->name)) {
                ERROR(structType->location);
            }

            for (auto decel : structType->declarations) {

                auto savedScope = currentScope;
                currentScope = new Scope();
                currentScope->parent = savedScope;
                auto structInner = enter0(decel);
                currentScope = savedScope;

                if (structInner == nullptr) {
                    ERROR(structType->location);
                }

                superStruct->types.push_back(structInner);

                auto identifier = structInner->identifier;
                if (identifier != nullptr) {
                    if (superStruct->map.find(identifier->value) != superStruct->map.end()) {
                        ERROR(identifier->location);
                    }

                    superStruct->map[identifier->value] = superStruct->types.size() - 1;
                }
            }

            if (structType->name != nullptr) {
                currentScope->setStruct(structType->name, superStruct);
            }
            semanticType = superStruct;
        }
    } else {
        switch (type->type) {
            case TYPE_VOID:
                semanticType = new SimpleType(TYPE_VOID, true);
                break;
            case TYPE_CHAR:
                semanticType = new SimpleType(TYPE_CHAR, true);
                break;
            case TYPE_INT:
                semanticType = new SimpleType(TYPE_INT, true);
                break;
        }
    }

    if (semanticType == nullptr) {
        ERROR(*location);
    }

    return enter(declarator, semanticType);
}