//
// Created by chris on 21.01.21.
//

#include "Types.h"
#include "TypeDefines.h"

extern SimpleType *const IntType = new SimpleType(TYPE_INT);
extern SimpleType *const CharType = new SimpleType(TYPE_CHAR);
extern SimpleType *const VoidType = new SimpleType(TYPE_VOID);
extern SuperType *const CharPointerType = new PointerType(CharType);
extern SuperType *const IntPointerType = new PointerType(IntType);
extern SuperType *const VoidPointerType = new PointerType(VoidType);
