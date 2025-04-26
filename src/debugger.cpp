#include <iostream>
#include <iomanip>

#include "../inc/debugger.h"

psx::Debugger::Debugger()
{
	m_traceFile.open("trace.txt");
	if (!m_traceFile.is_open())
	{
		std::cout << "ERROR: Debugger: Failed to create trace file\n";
	}

}

psx::Debugger::~Debugger()
{
	m_traceFile.close();
}

void psx::Debugger::logWarning(std::string message)
{
	m_traceFile << "WARNING: " << message << "\n";
}

void psx::Debugger::setPC(uint32_t pc)
{
	m_pc = pc;
}

void psx::Debugger::logRegisterTypeArithmetic(std::string mnemomic, 
	                                          uint32_t rd, 
	                                          uint32_t rs, 
	                                          uint32_t rt, 
	                                          int32_t result,
	                                          int32_t rsSrc, 
	                                          int32_t rtSrc)
{
	m_traceFile <<  LOG_PC(m_pc) << mnemomic << " r" << rd << ", r" << rs << ", r" << rt << " // " << result << ", " << LOG_SRC_RT_RS << "\n";
}


void psx::Debugger::logImmediateTypeArithmetic(std::string mnemomic,
	                                           uint32_t rt,
	                                           uint32_t rs,
	                                           int16_t immediate,
	                                           int32_t result,
	                                           int32_t rsSrc)
{
	m_traceFile << LOG_PC(m_pc) << mnemomic << LOG_RT_RS << immediate << " // " << result << ", " << rsSrc << ", " << immediate << "\n";
}

void psx::Debugger::logJump(std::string mnemonic, 
	                        uint32_t rt, 
	                        uint32_t rs, 
	                        int16_t offset, 
	                        bool jump, 
	                        uint32_t targetAddress, 
	                        int32_t rsSrc, 
	                        int32_t rtSrc)
{
	m_traceFile << LOG_PC(m_pc) << mnemonic << LOG_RT_RS << offset;
	if (jump)
	{
		m_traceFile << " -> Jump to 0x" << SET_ADDRES_STYLE << targetAddress << std::dec << " // " << LOG_SRC_RT_RS << "\n";
	}
	else
	{
		m_traceFile << " -> Jump is ignored" << " // " << LOG_SRC_RT_RS << "\n";
	}
}

void psx::Debugger::logDelayJump()
{
	m_traceFile << "Executing delayed jump -> 0x" << SET_ADDRES_STYLE << m_pc << "\n";
}