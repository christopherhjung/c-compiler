//
// Created by chris on 21.01.21.
//

#include "StringLiteral.h"
#include "../parser/PrettyPrinter.h"
#include "../transform/TransformContext.h"
#include "Util.h"

void StringLiteral::dump(PrettyPrinter &printer) {
    printer << *value;
}

llvm::Value *StringLiteral::createRightValue(TransformContext &context){
    std::string str = escapeString(*value);
    llvm::Value* globalPtr = context.builder.CreateGlobalStringPtr(str);
    return globalPtr;
}

llvm::Value * StringLiteral::getSize(TransformContext &context){
    return context.builder.getInt32(value->length() - 1);
}

/*
 *     std::string str = escapeString(*value);



    llvm::Value* globalPtr = context.builder.CreateGlobalString(str);

    llvm::ArrayType* arrayType = llvm::ArrayType::get(llvm::Type::getInt8Ty(context.llvmContext), str.length() + 1);

    llvm::Value *strArray = context.allocBuilder.CreateAlloca(llvm::PointerType::getUnqual(arrayType));

    context.builder.CreateStore(globalPtr, strArray);

    llvm::CallInst::CreateMalloc()

/*


    llvm::GlobalVariable* gvar_array__str = new llvm::GlobalVariable(, false, llvm::GlobalValue::PrivateLinkage, true);

    // Constant Definitions
    llvm::Constant *const_array_4 = llvm::ConstantDataArray::getString(context.llvmContext, str, true);
    std::vector<llvm::Constant*> const_ptr_5_indices;
    llvm::ConstantInt* const_int64_6 = llvm::ConstantInt::get(llvm::Type::getInt64Ty(context.llvmContext), 0);
    const_ptr_5_indices.push_back(const_int64_6);
    const_ptr_5_indices.push_back(const_int64_6);
    llvm::Constant* const_ptr_5 = llvm::ConstantExpr::getGetElementPtr( llvm::Type::getInt8PtrTy(context.llvmContext), gvar_array__str, const_ptr_5_indices);


    // Global Variable Definitions
    gvar_array__str->setInitializer(const_array_4);


* */