cmake_minimum_required(VERSION 3.21) 

project(psx VERSION 1.0)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(SOURCES
   src/main.cpp
   src/psx.cpp 
   src/context.cpp
   src/bus.cpp 
   src/debugger.cpp
   src/cpu/cpu.cpp
   src/cpu/instruction.cpp
   src/cpu/alu.cpp 
   src/cpu/gte.cpp
   inc/bus.h
   inc/context.h
   inc/cpu/cpu.h
   inc/debugger.h
   inc/debugger_constants.h
   inc/cpu/instruction.h
   inc/cpu/opcodes.h
   inc/cpu/gte.h
   inc/constants/cpu_constants.h
   inc/constants/gte_constants.h
   inc/psx.h
)

add_executable(psx ${SOURCES})

include_directories(inc)

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${SOURCES})