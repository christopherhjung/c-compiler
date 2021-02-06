//
// Created by chris on 21.01.21.
//
#pragma once

#include "ComplexType.h"
#include "MethodType.h"
#include "PointerType.h"
#include "ProxyType.h"
#include "SimpleType.h"
#include "SemanticStructType.h"
#include "SemanticType.h"
#include "PendingSemanticStructType.h"

extern SimpleType *const LongType;
extern SimpleType *const IntType;
extern SimpleType *const CharType;
extern SimpleType *const VoidType;
extern SimpleType *const BitType;
extern SemanticType *const CharPointerType ;
extern SemanticType *const IntPointerType;
extern SemanticType *const VoidPointerType;
extern SemanticType *const MethodTypeInstance;
extern SemanticType *const MethodPointerType;
