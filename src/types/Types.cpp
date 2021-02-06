//
// Created by chris on 21.01.21.
//

#include "Types.h"
#include "TypeDefines.h"

extern SimpleType *const LongType = new SimpleType(TYPE_LONG);
extern SimpleType *const IntType = new SimpleType(TYPE_INT);
extern SimpleType *const CharType = new SimpleType(TYPE_CHAR);
extern SimpleType *const VoidType = new SimpleType(TYPE_VOID);
extern SemanticType *const CharPointerType = new PointerType(CharType);
extern SemanticType *const IntPointerType = new PointerType(IntType);
extern SemanticType *const VoidPointerType = new PointerType(VoidType);
extern SemanticType *const MethodTypeInstance = new MethodType(nullptr);
extern SemanticType *const MethodPointerType = new PointerType(MethodTypeInstance);
