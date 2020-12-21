//
// Created by Christopher Jung on 21.12.20.
//

#pragma once


#include "../parser/SimpleObjects.h"
#include "../lexer/Location.h"


class SemanticException : public std::exception{
public:
    const Location& location;

    explicit SemanticException(const Location& location) : location(location){

    }
};

class Descriptor{
public:
    std::vector<Type*> types;
    bool filled = false;
    bool defined = false;
};

class Semantic {
public:
    std::unordered_map<const std::string*, Descriptor> defs;
    std::unordered_map<const DirectDeclarator*, Descriptor> defs2;

    void fatal(Element* element){
        Location& loc = element->location;

        throw SemanticException(loc);
    }

    void check(Unit *element) {

        for (auto child : element->children) {
            if (auto *declaration = dynamic_cast<Declaration *>(child)) {
                test(declaration, false);
            } else if (auto *method = dynamic_cast<Method *>(child)) {
                test(method->declaration, true);
            }
        }
    }

    void test(Declaration *declaration, bool isFilling) {
        if (declaration->declarator != nullptr) {
            if (auto *paramDeclarator = dynamic_cast<ParameterDirectDeclarator *>(declaration->declarator->directDeclarator)) {
                if (auto *identifier = dynamic_cast<Identifier *>(paramDeclarator->directDeclarator)) {
                    auto &def = defs[identifier->value];

                    auto &decls = paramDeclarator->parameterTypeList->declarations;

                    if (def.filled && isFilling) {
                        fatal(declaration);
                    }

                    if (def.defined) {
                        int minSize = std::min(decls.size(), def.types.size());

                        for (int i = 0; i < minSize; i++) {
                            if (decls[i]->type->type != def.types[i]->type) {
                                fatal(decls[i]->type);
                            }
                        }

                        if (decls.size() < def.types.size()) {
                            fatal(declaration);
                        } else if (decls.size() > def.types.size()) {
                            fatal(decls[minSize]->type);
                        }
                    } else {
                        for (auto *decl : decls) {
                            def.types.push_back(decl->type);
                        }

                        def.filled = isFilling;
                        def.defined = true;
                    }
                }
            }
        }
    }
};