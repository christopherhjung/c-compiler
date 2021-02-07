//
// Created by chris on 21.01.21.
//

#include "Sizeof.h"
#include "../parser/PrettyPrinter.h"
#include "Type.h"
#include "Declarator.h"
#include "../types/Types.h"
#include "../transform/TransformContext.h"
#include "llvm/IR/Constant.h"

void Sizeof::dump(PrettyPrinter &printer) {
    printer << "(";
    printer << "sizeof(";
    type->dump(printer);
    if (declarator != nullptr) {
        declarator->dump(printer);
    }
    printer << ")";
    printer << ")";
}

unsigned int Sizeof::getSize(TransformContext &context, const SemanticType* semanticType){
    llvm::Type* type = context.getType(semanticType);

    unsigned int size;
    if(type->isSized()){
        size = context.layout.getTypeAllocSize(type);
    }else{
        size = 1;
    }

    return size;
}

llvm::Constant* Sizeof::getInt32Size(TransformContext &context, const SemanticType* semanticType){
    return context.builder.getInt32( getSize(context, semanticType) );
}

llvm::Constant* Sizeof::getInt64Size(TransformContext &context, const SemanticType* semanticType){
    return context.builder.getInt64( getSize(context, semanticType) );
}

llvm::Value *Sizeof::createRightValue(TransformContext &context){
    return getInt32Size(context, inner);
}