#include <iomanip>

#include "../inc/debugger.h"

namespace psx
{
	Debugger::Debugger(std::filesystem::path outPath)
	{
#if !defined CONSOLE_OUTPUT
		m_traceFile.open(outPath);

		if (!m_traceFile.is_open())
		{
			std::cout << "ERROR: Debugger: Failed to create trace file\n";
		}
#endif
	}

	psx::Debugger::~Debugger()
	{
#if !defined CONSOLE_OUTPUT
		m_traceFile.close();
#endif
	}

	void Debugger::setMode(Mode mode)
	{
		m_mode = mode;
	}

	void Debugger::log(std::string message)
	{
		OUTPUT_STREAM << message << "\n";
	}

	void Debugger::logWarning(std::string message)
	{
		if (m_mode == Mode::DIS)
		{
			return;
		}
		OUTPUT_STREAM << "WARNING: " << message << "\n";
	}

	void Debugger::logPC(uint32_t pc)
	{
		if (m_mode == Mode::DIS)
		{
			OUTPUT_STREAM << "\n";
			return;
		}
		OUTPUT_STREAM << "PC=0x" << SET_ADDRES_STYLE << pc << "-> " << std::dec;
	}

	void Debugger::logDelayedBranch(uint32_t pc)
	{
		if (m_mode == Mode::DIS)
		{
			OUTPUT_STREAM << "\n";
			return;
		}

		OUTPUT_STREAM << "Executing delayed branch -> 0x" << SET_ADDRES_STYLE << pc << "\n";
	}
}