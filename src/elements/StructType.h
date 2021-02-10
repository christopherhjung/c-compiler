//
// Created by chris on 21.01.21.
//

#pragma once

#include <string>
#include "vector"
#include "Type.h"

class PrettyPrinter;
class Declaration;

class StructType : public Type {
public:
    const std::string *name;
    std::vector<Declaration *> declarations;

    void dump(PrettyPrinter &printer) override;
};


