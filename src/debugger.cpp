#include <iomanip>

#include "../inc/debugger.h"

namespace psx
{
	Debugger::Debugger()
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
	
	void Debugger::log(std::string message)
	{
		OUTPUT_STREAM << message << "\n";
	}

	void Debugger::logWarning(std::string message)
	{
		OUTPUT_STREAM << "WARNING: " << message << "\n";
	}

	// Get rid of this function and write one to log pc from cpu directly
	void Debugger::setPC(uint32_t pc)
	{
		m_pc = pc;
	}

	void Debugger::logPC(uint32_t pc)
	{
		OUTPUT_STREAM << "PC=0x" << SET_ADDRES_STYLE << pc << "-> " << std::dec;
	}

	void Debugger::logLoadShift(uint32_t requestedMemory,
		uint32_t alignedMemory,
		uint32_t memoryData,
		uint32_t rt,
		uint32_t rtSrc,
		uint32_t rtResult)
	{
		OUTPUT_STREAM << "                Requested memory: 0x" << SET_ADDRES_STYLE << requestedMemory << " -> Aligned memory: 0x" << SET_ADDRES_STYLE << alignedMemory << " = 0x" << memoryData << "\n";
		OUTPUT_STREAM << "                rt" << rt << ": 0x" << SET_ADDRES_STYLE << rtSrc << " -> 0x" << SET_ADDRES_STYLE << rtResult << "\n";
	}

	void Debugger::logLoadUpperImmediate(uint32_t rt, uint32_t immediate, uint32_t result)
	{
		OUTPUT_STREAM << "ldi rt" << rt << ", " << immediate << " // " << result << "\n";
	}

	void Debugger::logDelayedBranch()
	{
		OUTPUT_STREAM << "Executing delayed branch -> 0x" << SET_ADDRES_STYLE << m_pc << "\n";
	}

	void Debugger::logDelayedLoad(uint32_t rt, uint32_t rtSrc, bool sign, bool byte, bool halfword)
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

	void Debugger::logShiftLogical(const std::string& mnemonic,
		uint32_t rd,
		uint32_t rt,
		uint32_t sa,
		uint32_t rdSrc,
		uint32_t rtSrc)
	{
		OUTPUT_STREAM << mnemonic << " r" << rd << ", r" << rt << ", sa // " << rdSrc << ", " << rtSrc << ", " << sa << "\n";
	}

	void Debugger::logShiftVariable(const std::string& mnemonic,
		uint32_t rd,
		uint32_t rt,
		uint32_t rs,
		uint32_t rdSrc,
		uint32_t rtSrc,
		uint32_t rsSrc)
	{
		OUTPUT_STREAM << mnemonic << " r" << rd << ", r" << rt << ", r" << rs << " // " << rdSrc << ", " << rtSrc << ", " << rsSrc << "\n";
	}
}