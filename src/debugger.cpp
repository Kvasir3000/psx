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
	m_traceFile <<  LOG_PC(m_pc) << mnemomic << " r" << rd << ", r" << rs << ", r" << rt << " // " << result << ", " << LOG_SRC_RS_RT << "\n";
}


void psx::Debugger::logImmediateTypeArithmetic(std::string mnemomic,
	                                           uint32_t rt,
	                                           uint32_t rs,
	                                           int16_t immediate,
	                                           int32_t result,
	                                           int32_t rsSrc)
{
	m_traceFile << LOG_PC(m_pc) << mnemomic << LOG_RS_RT << immediate << " // " << result << ", " << rsSrc << ", " << immediate << "\n";
}

void psx::Debugger::logBranch(std::string mnemonic, 
	                          uint32_t rt, 
	                          uint32_t rs, 
	                          int16_t offset, 
	                          bool branch, 
	                          uint32_t targetAddress, 
	                          int32_t rsSrc, 
	                          int32_t rtSrc,
	                          bool compareToZero)
{
	m_traceFile << LOG_PC(m_pc) << mnemonic;
	if (!compareToZero)
	{
		m_traceFile << LOG_RS_RT;
	}
	else
	{
		m_traceFile << " r" << rs << ", 0, ";
	}
	m_traceFile << offset;

	if (branch && !compareToZero)
	{
		m_traceFile << " // " << LOG_SRC_RS_RT << " -> Branch to 0x" << SET_ADDRES_STYLE << targetAddress << std::dec << "\n";
	}
	else if (branch)
	{
		m_traceFile << " // " << rsSrc << ", 0 -> Branch to 0x" << SET_ADDRES_STYLE << targetAddress << std::dec << "\n";;

	}
	else
	{
		m_traceFile << " // " << LOG_SRC_RS_RT << " -> Branch is ignored\n";
	}
}

void psx::Debugger::logDelayBranch()
{
	m_traceFile << "Executing delayed branch -> 0x" << SET_ADDRES_STYLE << m_pc << "\n";
}