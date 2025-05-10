#pragma once 

#define SET_ADDRES_STYLE std::hex << std::setw(8) << std::setfill('0')

#define LOG_PC(pc) "PC=0x" << SET_ADDRES_STYLE << pc << "-> " << std::dec
#define LOG_RS_RT  " r" << rs << ", r" << rt
#define LOG_SRC_RS_RT rsSrc << ", " << rtSrc
#define LOG_SRC_RS_RT_SIGNED static_cast<int32_t>(rsSrc) << ", " << static_cast<int32_t>(rtSrc)
#define LOG_HI_LO "hi = " << hi << " lo = " << lo
#define LOG_HI_LO_SIGNED "hi = " << static_cast<int32_t>(hi) << " lo = " << static_cast<int32_t>(lo)   