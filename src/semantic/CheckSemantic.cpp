//
// Created by Christopher Jung on 21.12.20.
//

#include "CheckSemantic.h"
#include "../types/Types.h"
#include "../common.h"


void Semantic::checkType(const Expression *element, const SuperType *superType) {
    if (element->superType == nullptr || !element->superType->equals(superType)) {
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

            if (methodType->identifier != nullptr &&
                !currentScope->set(methodType->identifier->value, methodType, true)) {
                ERROR(methodType->identifier->location);
            }

            if (methodType->types.size() != 1 || !methodType->types[0]->equals(VoidType)) {
                for (int i = 0; i < methodType->types.size(); i++) {
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
    if (condition->superType->asSuperStructType()) {
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
    const SuperType *returnType;
    if (returnStatement->value == nullptr) {
        returnType = VoidType;
        loc = returnStatement;
    } else {
        enter(returnStatement->value);
        loc = returnStatement->value;
        returnType = returnStatement->value->superType;
    }

    if (!isAssignable(currentScope->getReturnType(), returnType)) {
        ERROR(loc->location);
    }
}

bool Semantic::isAssignable(const SuperType *target, const SuperType *source) {
    bool targetIsNumeric = target->equals(IntType) || target->equals(CharType);
    bool sourceIsNumeric = source->equals(IntType) || source->equals(CharType);

    return target->equals(source)
           || (targetIsNumeric && sourceIsNumeric)
           || (target->asPointerType() && sourceIsNumeric)
           || (target->equals(VoidPointerType) && source->asPointerType())
           || (source->equals(VoidPointerType) && target->asPointerType());
}

/*
void enter(Continue*){

}

void enter(Break*){
}*/

void Semantic::enter(Expression *expression) {
    enter0(expression);

    if (expression->superType == nullptr) {
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

        identifier->superType = desc->superType;
    } else if (auto constant = dynamic_cast<Constant *>(expression)) {
        constant->superType = new SimpleType(TYPE_CHAR, false);
    } else if (auto string = dynamic_cast<StringLiteral *>(expression)) {
        string->superType = new ProxyType(CharPointerType, false);
    } else if (auto call = dynamic_cast<Call *>(expression)) {
        for (auto expr : call->values) {
            enter(expr);
        }

#ifdef DEBUG
        if(auto printf = dynamic_cast<const Identifier*>(call->target)){
            if(*printf->value == "printf"){
                call->superType = VoidType;
                return;
            }
        }
#endif
        enter(call->target);
        auto methodType = call->target->superType->asMethodType();
        if(!methodType){
            methodType = call->target->superType->asPointerType()->subType->asMethodType();
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
                    if (!methodType->types[i]->equals(call->values[i]->superType)) {
                        ERROR(call->locations[i]);
                    }
                }
            }

            if (methodType->subType == nullptr) {
                ERROR(call->location);
            }

            call->superType = new ProxyType(methodType->subType, false);
            return;
        }

        ERROR(call->target->location);
    } else if (auto number = dynamic_cast<Number *>(expression)) {
        number->superType = IntType;
    } else if (auto select = dynamic_cast<Select *>(expression)) {
        enter(select->target);
        enter(select->index);

        auto rightType = select->index->superType;
        auto leftType = select->target->superType;

        if (auto pointer = leftType->asPointerType()) {
            if (!IntType->equals(rightType)) {
                ERROR(select->index->location);
            }

            select->superType = pointer->subType;
        } else if (auto pointer = rightType->asPointerType()) {
            if (!IntType->equals(leftType)) {
                ERROR(select->target->location);
            }

            select->superType = pointer->subType;
        }

    } else if (auto sizeOf = dynamic_cast<Sizeof *>(expression)) {
        sizeOf->inner = enter(sizeOf->declarator, sizeOf->type, &sizeOf->location);
        sizeOf->superType = IntType;
    } else if (auto ifSmall = dynamic_cast<IfSmall *>(expression)) {
        checkCondition(ifSmall->condition, ifSmall->condition->location);
        enter(ifSmall->left);
        enter(ifSmall->right);

        checkType(ifSmall->left, ifSmall->right->superType);

        ifSmall->superType = ifSmall->left->superType;
    } else {
        ERROR(expression->location);
    }
}

void Semantic::enter(Unary *unary) {
    enter(unary->value);

    auto type = unary->value->superType;
    switch (unary->op->id) {
        case STAR:
            if (auto dealloc = type->asPointerType()) {
                unary->superType = new ProxyType(dealloc->subType, true);
                return;
            }
            break;
        case PLUS:
            if (type->asPointerType()) {
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

void Semantic::enter(Binary *binary) {

    enter(binary->left);
    auto leftType = binary->left->superType;

    if (leftType == nullptr) {
        ERROR(binary->op->location);
    }

    switch (binary->op->id) {
        case ARROW:
            if (auto deallocType = leftType->asPointerType()) {
                if (auto superStruct = deallocType->subType->asSuperStructType()) {
                    if (auto identifier = dynamic_cast<Identifier *>(binary->right)) {
                        binary->superType = superStruct->member(identifier);
                        if (binary->superType != nullptr) {
                            return;
                        }
                    }
                }
            }
            ERROR(binary->op->location);
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
            break;
    }

    enter(binary->right);
    auto rightType = binary->right->superType;

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
                binary->superType = new SimpleType(TYPE_INT, false);
                return;
            }
            break;
        case PLUS:
            if (leftIsNumeric && rightIsNumeric) {
                binary->superType = new SimpleType(TYPE_INT, false);
                return;
            } else if (leftType->asPointerType() && rightIsInteger) {
                binary->opInfo = 1;
                binary->superType = new ProxyType(leftType, false);
                return;
            } else if (rightType->asPointerType() && leftIsInteger) {
                binary->opInfo = 2;
                binary->superType = new ProxyType(rightType, false);
                return;
            }
            break;
        case MINUS:
            if (leftIsNumeric && rightIsNumeric) {
                binary->superType = new SimpleType(TYPE_INT, false);
                return;
            } else if (leftType->asPointerType() && rightIsInteger) {
                binary->opInfo = 1;
                binary->superType = new ProxyType(leftType, false);
                return;
            } else if (leftType->asPointerType() && rightType->asPointerType() && leftType->equals(rightType)) {
                binary->opInfo = 2;
                binary->superType = new SimpleType(TYPE_INT, false);
                return;
            }
            break;
        case EQUAL:
        case NOT_EQUAL:
            if (leftIsNumeric && rightIsNumeric) {
                binary->superType = new SimpleType(TYPE_INT, false);
                return;
            } else if (leftType->asPointerType() && rightIsInteger) {
                binary->superType = new SimpleType(TYPE_INT, false);
                return;
            } else if (rightType->asPointerType() && leftIsInteger) {
                binary->superType = new SimpleType(TYPE_INT, false);
                return;
            } else if (rightType->equals(VoidPointerType) && leftType->asPointerType()) {
                binary->superType = new SimpleType(TYPE_INT, false);
                return;
            } else if (leftType->equals(VoidPointerType) && rightType->asPointerType()) {
                binary->superType = new SimpleType(TYPE_INT, false);
                return;
            } else if (leftType->equals(rightType)) {
                binary->superType = new SimpleType(TYPE_INT, false);
                return;
            }
            break;
        case LESS:
        case LESS_EQUAL:
        case GREATER:
        case GREATER_EQUAL:
            if (leftIsNumeric && rightIsNumeric) {
                binary->superType = new SimpleType(TYPE_INT, false);
                return;
            } else if (leftIsPointer && rightIsPointer) {
                binary->superType = new SimpleType(TYPE_INT, false);
                return;
            }
            break;

        case AND_AND:
        case OR_OR:
            if (leftIsComparable && rightIsComparable) {
                binary->superType = new SimpleType(TYPE_INT, false);
                return;
            }
            break;
        case LEFT_SHIFT:
        case RIGHT_SHIFT:
            if (leftIsInteger && rightIsInteger) {
                binary->superType = new SimpleType(TYPE_INT, false);
                return;
            }
            break;
        case ASSIGN:
            if (!leftType->isAssignable()) {
                ERROR_MSG(binary->op->location, binary->left->toString() + " not assignable");
            }

            if (isAssignable(leftType, rightType)) {
                binary->superType = rightType;
                return;
            }

            break;
    }

    ERROR(binary->op->location);
}

SuperType *Semantic::enter(DirectDeclarator *directDeclarator, SuperType *simpleType) {
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
        return simpleType;
    }

    return simpleType;
}

const SuperType *Semantic::enter0(Declaration *declaration) {
    if (declaration != nullptr) {
        auto type = enter(declaration);
        declaration->superType = type;
        if (type != nullptr) {
            if (type->asSimpleType() && type->identifier == nullptr) {
                ERROR(declaration->location);
            }

            if (type->identifier != nullptr && !currentScope->set(type->identifier->value, type, false)) {
                ERROR(type->identifier->location);
            }
        }

        return type;
    }

    return nullptr;
}


SuperType *Semantic::enter(Declaration *declaration) {
    return enter(declaration->declarator, declaration->type, &declaration->location);
}

SuperType *Semantic::enter(Declarator *declarator, Type* type, Location* location) {
    SuperType *superType = nullptr;
    if (auto structType = dynamic_cast<StructType *>(type)) {
        auto superStruct = new SuperStructType(true);

        if (structType->name != nullptr && structType->declarations.empty()) {
            if (!structType->declarations.empty()) {
                ERROR(structType->location);
            }

            superType = new PendingSuperStructType(structType->name, currentScope);
        } else {
            if (structType->name != nullptr && currentScope->getStruct(structType->name) != nullptr) {
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
            superType = superStruct;
        }
    } else {
        switch (type->type) {
            case TYPE_VOID:
                superType = new SimpleType(TYPE_VOID, true);
                break;
            case TYPE_CHAR:
                superType = new SimpleType(TYPE_CHAR, true);
                break;
            case TYPE_INT:
                superType = new SimpleType(TYPE_INT, true);
                break;
        }
    }

    if (superType == nullptr) {
        ERROR(*location);
    }

    return enter(declarator, superType);
}