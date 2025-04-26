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
)

add_executable(psx ${SOURCES})

include_directories(inc)

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${SOURCES})