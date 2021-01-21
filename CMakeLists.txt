cmake_minimum_required(VERSION 3.2)
project(c4)

set(CMAKE_CXX_STANDARD 17)


add_subdirectory(lib/googletest)

include_directories(lib/googletest)
include_directories(lib/googletest/googletest/include)
include_directories(lib/googletest/googlemock/include)

#SET(CMAKE_EXE_LINKER_FLAGS "-L/media/hd/workspace/c4/llvm/install/lib  -lz -lrt -ldl -lpthread -lm -pthread -lLLVMCore -lLLVMRemarks -lLLVMBitstreamReader -lLLVMBinaryFormat -lLLVMSupport -lLLVMDemangle")
SET(CMAKE_CXX_FLAGS "-Wall -W -std=c++17 -O0 -g -Wno-unused-parameter -isystem/media/hd/workspace/c4/llvm/install/include -D_GNU_SOURCE -D_DEBUG -D__STDC_CONSTANT_MACROS -D__STDC_FORMAT_MACROS -D__STDC_LIMIT_MACROS -pthread")
#SET(CMAKE_C_FLAGS "-Wall -W -std=c++17 -O0 -g -Wno-unused-parameter -isystem/media/hd/workspace/c4/llvm/install/include -D_GNU_SOURCE -D_DEBUG -D__STDC_CONSTANT_MACROS -D__STDC_FORMAT_MACROS -D__STDC_LIMIT_MACROS")

#add_compile_options("-Wall -W -std=c++17 -O0 -g -Wno-unused-parameter -isystem/media/hd/workspace/c4/llvm/install/include -D_GNU_SOURCE -D_DEBUG -D__STDC_CONSTANT_MACROS -D__STDC_FORMAT_MACROS -D__STDC_LIMIT_MACROS")


file(GLOB_RECURSE source_list "*.cpp" )

link_directories(/media/hd/workspace/c4/llvm/install/lib)
include_directories(/media/hd/workspace/c4/llvm/install/include/)
link_libraries(z rt dl pthread m LLVMCore LLVMRemarks LLVMBitstreamReader LLVMBinaryFormat LLVMSupport LLVMDemangle)

add_executable(c4 ${source_list})
add_executable(build src/build.cpp src/transform/TransformContext.cpp src/transform/TransformContext.h src/transform/TransformException.cpp src/transform/TransformException.h src/elements/Block.cpp src/elements/Block.h src/elements/Statement.cpp src/elements/Statement.h src/types/SuperType.cpp src/types/SuperType.h src/elements/Identifier.cpp src/elements/Identifier.h src/elements/Expression.cpp src/elements/Expression.h src/elements/Binary.cpp src/elements/Binary.h src/elements/Operator.cpp src/elements/Operator.h src/elements/Element.cpp src/elements/Element.h src/elements/Dumpable.cpp src/elements/Dumpable.h src/elements/CompareBinary.cpp src/elements/CompareBinary.h src/elements/Assignment.cpp src/elements/Assignment.h src/elements/LogicalAnd.cpp src/elements/LogicalAnd.h src/elements/LogicalOr.cpp src/elements/LogicalOr.h src/elements/Unary.cpp src/elements/Unary.h src/elements/Return.cpp src/elements/Return.h src/elements/Type.cpp src/elements/Type.h src/elements/StringLiteral.cpp src/elements/StringLiteral.h src/elements/Constant.cpp src/elements/Constant.h src/elements/Number.cpp src/elements/Number.h src/elements/Continue.cpp src/elements/Continue.h src/elements/Break.cpp src/elements/Break.h src/elements/GoTo.cpp src/elements/GoTo.h src/elements/LabeledStatement.cpp src/elements/LabeledStatement.h src/elements/Util.cpp src/elements/Util.h src/elements/Declarator.cpp src/elements/Declarator.h src/elements/Declaration.cpp src/elements/Declaration.h src/elements/ParameterTypeList.cpp src/elements/ParameterTypeList.h src/elements/ParameterDirectDeclarator.cpp src/elements/ParameterDirectDeclarator.h src/elements/StructType.cpp src/elements/StructType.h src/elements/Method.cpp src/elements/Method.h src/elements/Call.cpp src/elements/Call.h src/elements/Sizeof.cpp src/elements/Sizeof.h src/elements/Select.cpp src/elements/Select.h src/elements/IfSmall.cpp src/elements/IfSmall.h src/elements/If.cpp src/elements/If.h src/elements/While.cpp src/elements/While.h src/elements/Unit.cpp src/elements/Unit.h src/types/ComplexType.cpp src/types/ComplexType.h src/types/MethodType.cpp src/types/MethodType.h src/types/PointerType.cpp src/types/PointerType.h src/types/SuperStructType.cpp src/types/SuperStructType.h src/types/SimpleType.cpp src/types/SimpleType.h src/types/ProxyType.cpp src/types/ProxyType.h src/elements/Elements.h src/elements/DirectDeclarator.cpp src/elements/DirectDeclarator.h src/types/Types.h)

add_executable(testC4 tests/main.cpp)

target_link_libraries(testC4 gtest gtest_main)

add_executable(llvmExample examples/custom.cpp)
