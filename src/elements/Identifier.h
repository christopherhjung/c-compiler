//
// Created by chris on 21.01.21.
//


#include <string>
#include "../parser/PrettyPrinter.h"
#include "Expression.h"
#include "DirectDeclarator.h"
class Identifier : public Expression, public DirectDeclarator{
public:
    const std::string *value = nullptr;

    Identifier(){}
    Identifier(const std::string* value) : value(value){}

    void dump(PrettyPrinter& printer) override;

    bool operator==(const Identifier &rhs) const {
        return value == rhs.value;
    }

    bool operator!=(const Identifier &rhs) const {
        return !(rhs == *this);
    }

    llvm::Value* createRightValue(TransformContext &context) override;
    llvm::Value* createLeftValue(TransformContext &context) override;
};



