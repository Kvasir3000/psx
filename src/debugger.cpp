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

// Get rid of this function and write one to log pc from cpu directly
void psx::Debugger::setPC(uint32_t pc)
{
	m_pc = pc;
}

void psx::Debugger::logPC(uint32_t pc)
{
	OUTPUT_STREAM << "PC=0x" << SET_ADDRES_STYLE << pc << "-> " << std::dec;
}

void psx::Debugger::logBranch(const std::string& mnemonic, 
	                          uint32_t rt, 
	                          uint32_t rs, 
	                          int16_t offset, 
	                          bool branch, 
	                          uint32_t targetAddress, 
	                          int32_t rsSrc, 
	                          int32_t rtSrc,
	                          bool compareToZero)
{
	OUTPUT_STREAM << mnemonic;
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

void psx::Debugger::logJump(const std::string& mnemonic, uint32_t targetAddress)
{
	OUTPUT_STREAM << mnemonic << " -> Jump to 0x" << SET_ADDRES_STYLE << targetAddress << "\n";
}

void psx::Debugger::logJumpRegister(const std::string& mnemonic, uint32_t rs, uint32_t rsSrc)
{
	OUTPUT_STREAM << mnemonic << " r" << rs << " -> Jump to 0x" << SET_ADDRES_STYLE << rsSrc << "\n";
}

void psx::Debugger::logMemoryOperation(const std::string& mnemonic, uint32_t rt, int32_t offset, uint32_t base, uint32_t baseSrc)
{
	OUTPUT_STREAM << mnemonic << " r" << rt << ", offset(r" << base << ") // offset = " << offset << ", base = 0x" << SET_ADDRES_STYLE << baseSrc << "\n";
}


void psx::Debugger::logLoadShift(uint32_t requestedMemory,
	                             uint32_t alignedMemory, 
	                             uint32_t memoryData, 
	                             uint32_t rt, 
	                             uint32_t rtSrc, 
	                             uint32_t rtResult)
{
	OUTPUT_STREAM << "                Requested memory: 0x" << SET_ADDRES_STYLE << requestedMemory << " -> Aligned memory: 0x" << SET_ADDRES_STYLE << alignedMemory << " = 0x" << memoryData << "\n";
	OUTPUT_STREAM << "                rt" << rt << ": 0x" << SET_ADDRES_STYLE << rtSrc << " -> 0x" << SET_ADDRES_STYLE << rtResult << "\n";
}

void psx::Debugger::logLoadUpperImmediate(uint32_t rt, uint32_t immediate, uint32_t result)
{
	OUTPUT_STREAM  << "ldi rt" << rt << ", " << immediate << " // " << result << "\n"; 
}

void psx::Debugger::logDelayedBranch()
{
	OUTPUT_STREAM << "Executing delayed branch -> 0x" << SET_ADDRES_STYLE << m_pc << "\n";
}

void psx::Debugger::logDelayedLoad(uint32_t rt, uint32_t rtSrc, bool sign, bool byte, bool halfword)
{
	OUTPUT_STREAM << "Executing delayed load -> r" << std::dec << rt << " = ";

	if (!sign)
	{
		OUTPUT_STREAM << rtSrc << "\n";
	}
	else if (byte)
	{
		OUTPUT_STREAM << static_cast<int32_t>(static_cast<int8_t>(rtSrc)) << "\n";
	}
	else if (halfword)
	{
		OUTPUT_STREAM << static_cast<int32_t>(static_cast<int16_t>(rtSrc)) << "\n";
	}
}

void psx::Debugger::logMove(const std::string& mnemonic, uint32_t rt, uint32_t rd, int32_t rdSrc)
{
	if _UNLIKELY((rd == HI || rd == LO))
	{

		OUTPUT_STREAM << mnemonic << " r" << rt << ", " << ((rd == HI) ? "hi" : "lo") << " // " << rdSrc << "\n";
	}
	else
	{
		OUTPUT_STREAM << mnemonic << " r" << rt << ", r" << rd << " // " << rdSrc << "\n";
	}
}

void psx::Debugger::logShiftLogical(const std::string& mnemonic, 
	                               uint32_t rd, 
	                               uint32_t rt, 
	                               uint32_t sa, 
	                               uint32_t rdSrc, 
	                               uint32_t rtSrc)
{
	OUTPUT_STREAM << mnemonic << " r" << rd << ", r" << rt << ", sa // " << rdSrc << ", " << rtSrc << ", " << sa << "\n";
}

void psx::Debugger::logShiftVariable(const std::string& mnemonic,
	                                uint32_t rd,
	                                uint32_t rt,
	                                uint32_t rs,
	                                uint32_t rdSrc,
	                                uint32_t rtSrc,
	                                uint32_t rsSrc)
{
	OUTPUT_STREAM << mnemonic << " r" << rd << ", r" << rt << ", r" << rs << " // " << rdSrc << ", " << rtSrc << ", " << rsSrc << "\n";
}
