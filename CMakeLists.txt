cmake_minimum_required(VERSION 3.21) 

project(psx VERSION 1.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(SOURCES
   tests/test.cpp
   src/main.cpp
   src/psx.cpp 
   src/context.cpp
   src/bus.cpp 
   src/debugger.cpp
   src/cpu/cpu.cpp
   src/cpu/instruction.cpp
   src/cpu/alu.cpp 
   src/cpu/gte.cpp
   src/cpu/cop0.cpp
   tests/testbuilder.cpp
   tests/test.h
   tests/testsuits.h
   tests/testbuilder.h
   inc/bus.h
   inc/context.h
   inc/cpu/cpu.h
   inc/cpu/operands.h
   inc/debugger.h
   inc/cpu/instruction.h
   inc/cpu/opcodes.h
   inc/cpu/gte.h
   inc/cpu/cop.h
   inc/constants/debugger_constants.h
   inc/constants/cpu_constants.h
   inc/constants/gte_constants.h
   inc/psx.h
   inc/cpu/cop0.h
)

add_executable(psx ${SOURCES})

include_directories(inc)

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${SOURCES})