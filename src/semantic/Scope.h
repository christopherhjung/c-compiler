//
// Created by chris on 21.01.21.
//

#pragma once

#include <unordered_map>
#include <string>

class SuperType;

class SuperStructType;

template<class T>
class Descriptor {
public:
    T *superType = nullptr;
    bool implementation = false;
    bool defined = false;

    Descriptor() {

    }

    Descriptor(const Descriptor *other) : superType(other->superType), implementation(other->implementation),
                                          defined(other->defined) {

    }

    Descriptor(SuperType *superType) : superType(superType) {

    }
};

class Scope {
public:
    std::unordered_map<const std::string *, Descriptor<SuperType>> types;
    std::unordered_map<const std::string *, Descriptor<SuperStructType>> structs;
    std::unordered_map<const std::string *, int> labels;
    Scope *parent = nullptr;
    const SuperType *returnType = nullptr;

    Scope() {}

    bool isLabel(const std::string *label);

    void setLabel(const std::string *label);

    const SuperType *getReturnType();

    Descriptor<SuperStructType> *getStruct(const std::string *str);

    bool setStruct(const std::string *str, SuperStructType *type);

    bool set(const std::string *str, SuperType *type, bool hasImplementation);

    Descriptor<SuperType> *get(const std::string *str);
};

