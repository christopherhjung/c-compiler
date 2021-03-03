//
// Created by Christopher Jung on 21.12.20.
//

#include "CheckSemantic.h"

void Semantic::check(Unit *element) {
    element->scope = mainScope;
    for (auto child : element->children) {
        if (auto *declaration = dynamic_cast<Declaration *>(child)) {
            enter0(declaration);
        } else if (auto *method = dynamic_cast<Method *>(child)) {
            auto methodType = enter0(method->declaration)->asMethodType();

            auto inner = new Scope();
            if (methodType->identifier != nullptr &&
                !currentScope->set(methodType->identifier, true)) {

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
                        if (!inner->set(identifier, false)) {
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
    }
}

void Semantic::checkCondition(Expression *condition, Location &location) {
    enter(condition);
    if (condition->getType()->asSemanticStructType()) {
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
    const SemanticType *returnType;
    if (returnStatement->value == nullptr) {
        loc = returnStatement;
        returnType = VoidType;
    } else {
        enter(returnStatement->value);
        loc = returnStatement->value;
        returnType = returnStatement->value->getType();
    }

    returnStatement->setType(methodScope->returnType);
    if (!isAssignable(currentScope->getReturnType(), returnType)) {
        ERROR(loc->location);
    }
}

bool Semantic::isAssignable(const SemanticType *target, const SemanticType *source) {
    target = target->packMethodType();
    source = source->packMethodType();

    bool targetIsNumeric = target->equals(IntType) || target->equals(CharType);
    bool sourceIsNumeric = source->equals(IntType) || source->equals(CharType);

    bool assignable = target->equals(source)
           || (targetIsNumeric && sourceIsNumeric)
           || (target->asPointerType() && sourceIsNumeric)
           || (target->equals(VoidPointerType) && source->asPointerType())
           || (source->equals(VoidPointerType) && target->asPointerType());

    return assignable;
}

/*
void enter(Continue*){

}

void enter(Break*){
}*/

void Semantic::enter(Expression *expression) {
    enter0(expression);

    if (expression->getType() == nullptr) {
        enter0(expression);
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
    } else if (auto identifier = dynamic_cast<IdentifierUse *>(expression)) {
        auto desc = currentScope->get(identifier->value);

        if (!desc->defined) {
            ERROR(identifier->location);
        }

        identifier->identifier = desc->identifier;
        //identifier->setType(desc->semanticType);
    } else if (auto constant = dynamic_cast<Constant *>(expression)) {
        constant->setType(IntType);
    } else if (auto string = dynamic_cast<StringLiteral *>(expression)) {
        string->setType(CharPointerType);
    } else if (auto call = dynamic_cast<Call *>(expression)) {
        for (auto expr : call->values) {
            enter(expr);
        }

        enter(call->target);
        auto methodType = call->target->getType()->unpackMethodType();
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
                    if(!isAssignable(methodType->types[i], call->values[i]->getType())){
                        ERROR(call->locations[i]);
                    }
                }
            }

            if (methodType->subType == nullptr) {
                ERROR(call->location);
            }

            call->setType(new ProxyType(methodType->subType, false));
            return;
        }

        ERROR(call->target->location);
    } else if (auto number = dynamic_cast<Number *>(expression)) {
        number->setType(IntType);
    } else if (auto select = dynamic_cast<Select *>(expression)) {
        enter(select->target);
        enter(select->index);

        auto rightType = select->index->getType();
        auto leftType = select->target->getType();

        if (auto pointer = leftType->asPointerType()) {
            if (!IntType->equals(rightType)) {
                ERROR(select->index->location);
            }

            select->setType(pointer->subType);
        } else if (auto pointer = rightType->asPointerType()) {
            if (!IntType->equals(leftType)) {
                ERROR(select->target->location);
            }

            select->setType(pointer->subType);
        }

    } else if (auto sizeOf = dynamic_cast<Sizeof *>(expression)) {
        sizeOf->inner = enter(sizeOf->declarator, sizeOf->type, &sizeOf->location);
        sizeOf->setType(IntType);
    } else if (auto choose = dynamic_cast<Choose *>(expression)) {
        checkCondition(choose->condition, choose->condition->location);
        enter(choose->left);
        enter(choose->right);

        auto leftType = choose->left->getType()->packMethodType();
        if(leftType == nullptr){
            ERROR(choose->left->location);
        }

        auto rightType = choose->right->getType()->packMethodType();
        if(rightType == nullptr){
            ERROR(choose->right->location);
        }

        if (!leftType->equals(rightType)) {
            ERROR(choose->location);
        }

        choose->setType(choose->left->getType());
    } else {
        ERROR(expression->location);
    }
}

void Semantic::enter(Unary *unary) {
    enter(unary->value);

    auto type = unary->value->getType();


    bool isInteger = IntType->equals(type) ;
    bool isNumeric = isInteger || type->equals(CharType);
    bool isStruct = type->asSemanticStructType();
    bool leftIsComparable = !(isStruct || VoidType->equals(type));

    switch (unary->op->id) {
        case STAR:
            if (auto dealloc = type->asPointerType()) {
                if(dealloc->asMethodType()){
                    unary->setType(new ProxyType(dealloc, true));
                }else{
                    unary->setType( new ProxyType(dealloc->subType, true));
                }
                return;
            }else if(type->asMethodType()){
                unary->setType( new ProxyType(type, false));
                return;
            }
            break;
        case PLUS:
        case MINUS:
            if(isNumeric){
                unary->setType(new ProxyType(type, false));
                return;
            }
            break;
        case NOT:
            if(leftIsComparable){
                unary->setType(IntType);
                return;
            }
            break;
        case SIZEOF:
            unary->setType(IntType);
            return;
        case AND:
            unary->setType(new PointerType(type));
            return;
    }

    ERROR(unary->op->location);
}

void Semantic::enter(Binary *binary) {
    enter(binary->left);
    auto leftType = binary->left->getType()->packMethodType();

    switch (binary->op->id) {
        case ARROW:
            if (auto deallocType = leftType->asPointerType()) {
                if (auto superStruct = deallocType->subType->asSemanticStructType()) {
                    if (auto identifier = dynamic_cast<IdentifierUse *>(binary->right)) {
                        binary->setType( superStruct->member(identifier));
                        if (binary->getType() != nullptr) {
                            return;
                        }
                    }
                }
            }
            ERROR(binary->op->location);
            break;
        case DOT:
            if (auto superStruct = leftType->asSemanticStructType()) {
                if (auto identifier = dynamic_cast<IdentifierUse *>(binary->right)) {
                    binary->setType(superStruct->member(identifier));
                    if (binary->getType() != nullptr) {
                        return;
                    }
                }
            }
            ERROR(binary->op->location);
            break;
    }

    enter(binary->right);
    auto rightType = binary->right->getType()->packMethodType();

    bool leftIsInteger = IntType->equals(leftType);
    bool rightIsInteger = IntType->equals(rightType) ;

    bool leftIsPointer = IntType->equals(leftType);
    bool rightIsPointer = IntType->equals(rightType);

    bool leftIsNumeric = leftIsInteger || leftType->equals(CharType);
    bool rightIsNumeric = rightIsInteger || rightType->equals(CharType);

    bool leftIsStruct = leftType->asSemanticStructType();
    bool rightIsStruct = rightType->asSemanticStructType();

    bool leftIsComparable = !(leftIsStruct || VoidType->equals(leftType));
    bool rightIsComparable = !(rightIsStruct || VoidType->equals(rightType));


    switch (binary->op->id) {
        case STAR:
            if (leftIsNumeric && rightIsNumeric) {
                binary->setType(IntType);
                return;
            }
            break;
        case PLUS:
            if (leftIsNumeric && rightIsNumeric) {
                binary->setType(IntType);
                return;
            } else if (leftType->asPointerType() && rightIsInteger) {
                binary->opInfo = 1;
                binary->setType(new ProxyType(leftType, false));
                return;
            } else if (rightType->asPointerType() && leftIsInteger) {
                binary->opInfo = 2;
                binary->setType(new ProxyType(rightType, false));
                return;
            }
            break;
        case MINUS:
            if (leftIsNumeric && rightIsNumeric) {
                binary->setType(IntType);
                return;
            } else if (leftType->asPointerType() && rightIsInteger) {
                binary->opInfo = 1;
                binary->setType(new ProxyType(leftType, false));
                return;
            } else if (leftType->asPointerType() && rightType->asPointerType() && leftType->equals(rightType)) {
                binary->opInfo = 2;
                binary->setType(IntType);
                return;
            }
            break;
        case EQUAL:
        case NOT_EQUAL:
            if (leftIsNumeric && rightIsNumeric) {
                binary->setType(IntType);
                return;
            } else if (leftType->asPointerType() && rightIsInteger) {
                binary->setType(IntType);
                return;
            } else if (rightType->asPointerType() && leftIsInteger) {
                binary->setType(IntType);
                return;
            } else if (rightType->equals(VoidPointerType) && leftType->asPointerType()) {
                binary->setType(IntType);
                return;
            } else if (leftType->equals(VoidPointerType) && rightType->asPointerType()) {
                binary->setType(IntType);
                return;
            } else if (leftType->equals(rightType)) {
                binary->setType(IntType);
                return;
            }
            break;
        case LESS:
        case LESS_EQUAL:
        case GREATER:
        case GREATER_EQUAL:
            if (leftIsNumeric  && rightIsNumeric) {
                binary->setType(IntType);
                return;
            } else if (leftIsPointer && rightIsPointer) {
                binary->setType(IntType);
                return;
            }
            break;

        case AND_AND:
        case OR_OR:
            if (leftIsComparable && rightIsComparable) {
                binary->setType(IntType);
                return;
            }
            break;
        case LEFT_SHIFT:
        case RIGHT_SHIFT:
            if (leftIsInteger && rightIsInteger) {
                binary->setType(IntType);
                return;
            }
            break;
        case ASSIGN:
            if (!leftType->isAssignable()) {
                ERROR_MSG(binary->op->location, binary->left->toString() + " not assignable");
            }

            if (isAssignable(leftType, rightType)) {
                binary->setType(leftType);
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
        identifier->setType(simpleType);
        return simpleType;
    }

    return simpleType;
}

const SemanticType *Semantic::enter0(Declaration *declaration) {
    if (declaration != nullptr) {
        auto type = enter(declaration);
        declaration->setType(type);
        if (type != nullptr) {
            if (type->asSimpleType() && type->identifier == nullptr) {
                ERROR(declaration->location);
            }

            if (type->identifier != nullptr) {
                if(!currentScope->set(type->identifier, false)){
                    ERROR(type->identifier->location);
                }
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

        auto rightType = declaration->initializer->getType()->packMethodType();

        if (!isAssignable(leftType, rightType)) {
            ERROR(declaration->location);
        }
    }
    return leftType;
}

SemanticType *Semantic::enter(Declarator *declarator, Type* type, Location* location) {
    SemanticType *semanticType = nullptr;
    if (auto structType = dynamic_cast<StructType *>(type)) {

        bool defining = !structType->declarations.empty();
        bool hasName = structType->name != nullptr;

        SemanticStructType *semanticStruct = nullptr;
        if (hasName) {
            if(defining){
                if(currentScope->structDeclaredInScope(structType->name)){
                    semanticStruct = currentScope->getStruct(structType->name)->semanticType;

                    if(semanticStruct->defined){
                        //already defined in scope
                        ERROR(structType->location);
                    }
                }else{
                    semanticStruct = new SemanticStructType(true);

                    if (!currentScope->setStruct(structType->name, semanticStruct)) {
                        ERROR(structType->location);
                    }
                }
            }else{
                auto desc = currentScope->getStruct(structType->name);
                if (desc != nullptr) {
                    semanticStruct = desc->semanticType;
                }else{
                    semanticStruct = new SemanticStructType(true);

                    if(!currentScope->setStruct(structType->name, semanticStruct)){
                        ERROR(structType->location);
                    }
                }
            }
        }else{
            semanticStruct = new SemanticStructType(true);
        }

        if (!defining) {
            semanticType = new ProxyType(semanticStruct, true);
        } else {
            if(semanticStruct->defined){
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

                semanticStruct->types.push_back(structInner);

                auto identifier = structInner->identifier;
                if (identifier != nullptr) {
                    if (semanticStruct->map.find(identifier->value) != semanticStruct->map.end()) {
                        ERROR(identifier->location);
                    }

                    semanticStruct->map[identifier->value] = semanticStruct->types.size() - 1;
                }
            }

            semanticStruct->defined = true;
            semanticType = semanticStruct;
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