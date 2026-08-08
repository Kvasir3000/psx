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

	void Debugger::logDelayedBranch()
	{
		OUTPUT_STREAM << "Executing delayed branch -> 0x" << SET_ADDRES_STYLE << m_pc << "\n";
	}
}