//
// Created by chris on 21.01.21.
//

#include "PointerType.h"
#include "ComplexType.h"
#include "SuperType.h"

bool PointerType::equals(const SuperType *other) const {
    if( auto simpleOther = other->asPointerType() ){
        return subType->equals(simpleOther->subType);
    }
    return false;
}
