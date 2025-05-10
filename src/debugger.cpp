#include <iostream>
#include <iomanip>

#include "../inc/debugger.h"

psx::Debugger::Debugger()
{
#if !defined CONSOLE_OUTPUT
	m_traceFile.open("trace.txt");

	if (!m_traceFile.is_open())
	{
		std::cout << "ERROR: Debugger: Failed to create trace file\n";
	}
#endif
}

psx::Debugger::~Debugger()
{
#if !defined CONSOLE_OUTPUT
	OUTPUT_STREAM.close();
#endif
}

void psx::Debugger::logWarning(std::string message)
{
	OUTPUT_STREAM << "WARNING: " << message << "\n";
}

void psx::Debugger::setPC(uint32_t pc)
{
	m_pc = pc;
}

// Implement proper sign display
void psx::Debugger::logRegisterTypeArithmetic(std::string mnemonic,
	                                          uint32_t rd, 
	                                          uint32_t rs, 
	                                          uint32_t rt, 
	                                          uint32_t result,
	                                          uint32_t rsSrc, 
	                                          uint32_t rtSrc, 
	                                          bool sign)
{
	OUTPUT_STREAM << LOG_PC(m_pc) << mnemonic << " r" << rd << "," << LOG_RS_RT << rt << " // ";
	if (sign)
	{
		OUTPUT_STREAM << static_cast<int32_t>(result) << ", " << LOG_SRC_RS_RT_SIGNED << "\n";
	}
	else
	{
		OUTPUT_STREAM << result << ", " << LOG_SRC_RS_RT << "\n";
	}
}

void psx::Debugger::logRegisterTypeMultiplicativeArithmetic(std::string mnemonic, 
	                                                       uint32_t rs, 
	                                                       uint32_t rt, 
	                                                       uint32_t hi, 
	                                                       uint32_t lo, 
	                                                       uint32_t rsSrc, 
	                                                       uint32_t rtSrc,
	                                                       bool sign)
{
	OUTPUT_STREAM << LOG_PC(m_pc) << mnemonic << LOG_RS_RT << " // ";
	if (sign)
	{
		OUTPUT_STREAM << LOG_HI_LO_SIGNED << ", " << LOG_SRC_RS_RT_SIGNED << "\n";
	}
	else
	{
		OUTPUT_STREAM << LOG_HI_LO << ", " << LOG_SRC_RS_RT << "\n";
	}
}


void psx::Debugger::logImmediateTypeArithmetic(std::string mnemonic,
	                                           uint32_t rt,
	                                           uint32_t rs,
	                                           int16_t immediate,
	                                           int32_t result,
	                                           int32_t rsSrc)
{
	OUTPUT_STREAM << LOG_PC(m_pc) << mnemonic << LOG_RS_RT << ", " << immediate << " // " << result << ", " << rsSrc << ", " << immediate << "\n";
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
	OUTPUT_STREAM << LOG_PC(m_pc) << mnemonic;
	if (!compareToZero)
	{
		OUTPUT_STREAM << LOG_RS_RT << ". ";
	}
	else
	{
		OUTPUT_STREAM << " r" << rs << ", 0, ";
	}
	OUTPUT_STREAM << offset;

	if (branch && !compareToZero)
	{
		OUTPUT_STREAM << " // " << LOG_SRC_RS_RT << " -> Branch to 0x" << SET_ADDRES_STYLE << targetAddress << std::dec << "\n";
	}
	else if (branch)
	{
		OUTPUT_STREAM << " // " << rsSrc << ", 0 -> Branch to 0x" << SET_ADDRES_STYLE << targetAddress << std::dec << "\n";;
	}
	else
	{
		OUTPUT_STREAM << " // " << LOG_SRC_RS_RT << " -> Branch is ignored\n";
	}
}

void psx::Debugger::logDelayBranch()
{
	OUTPUT_STREAM << "Executing delayed branch -> 0x" << SET_ADDRES_STYLE << m_pc << "\n";
}

void psx::Debugger::logMove(std::string mnemonic, uint32_t rt, uint32_t rd, int32_t rdSrc)
{
	OUTPUT_STREAM << LOG_PC(m_pc) << mnemonic << " r" << rt << ", r" <<  rd << " // " << rdSrc << "\n";
}