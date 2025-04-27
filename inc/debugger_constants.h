#pragma once 

#define SET_ADDRES_STYLE std::hex << std::setw(8) << std::setfill('0')

#define LOG_PC(pc) "PC=0x" << SET_ADDRES_STYLE << pc << "-> " << std::dec
#define LOG_RS_RT  " r" << rs << ", r" << rt << ", "
#define LOG_SRC_RS_RT rsSrc << ", " << rtSrc