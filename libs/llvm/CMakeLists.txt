set(LIBRARY_NAME llvm)

add_library(${LIBRARY_NAME}
        IR/Common.hpp
        IR/Module.cpp
        IR/BasicBlock.cpp
        IR/Function.cpp
        IR/Value.cpp
        IR/Instructions.cpp
        IR/Instructions.hpp
        IR/BasicBlock.hpp
        IR/User.hpp
        IR/Module.hpp
        IR/Module.cpp
        IR/LLVMContext.cpp
        IR/LLVMContext.hpp
        IR/Use.cpp
        IR/Use.hpp
        IR/User.cpp
        IR/Value.hpp
        IR/Function.hpp
        IR/Constant.hpp
        IR/Constant.cpp
        IR/GlobalValue.hpp
        IR/GlobalValue.cpp
        IR/GlobalVariable.hpp
        IR/GlobalVariable.cpp
        IR/GlobalVariable.hpp
        IR/Type.cpp
        IR/Type.hpp
        IR/ConstantData.cpp
        IR/ConstantData.hpp
        IR/IRPrinter.hpp
        IR/Argument.hpp
        IR/Argument.cpp
        IR/GlobalString.hpp
        IR/GlobalString.cpp

)

target_include_directories(${LIBRARY_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})