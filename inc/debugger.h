#pragma once

#include <fstream>
#include <stdint.h>
#include <string>

#include "../inc/debugger_constants.h"

#define CONSOLE_OUTPUT

namespace psx
{
	class Debugger
	{
	private:
#if defined CONSOLE_OUTPUT
        #define OUTPUT_STREAM std::cout 
#else 
		std::ofstream m_traceFile;
        #define OUTPUT_STREAM m_traceFile
#endif
		uint32_t      m_pc;
	public:
		Debugger();
		~Debugger();

		void logWarning(std::string message);

		void setPC(uint32_t pc);
		void logRegisterTypeArithmetic(std::string mnemonic, uint32_t rd, uint32_t rs, uint32_t rt, uint32_t result, uint32_t rsSrc, uint32_t rtSrc, bool sign);
		void logRegisterTypeMultiplicativeArithmetic(std::string mnemonic, uint32_t rs, uint32_t rt, uint32_t hi, uint32_t lo, uint32_t rsSrc, uint32_t rtSrc, bool sign);
		void logImmediateTypeArithmetic(std::string mnemonic, uint32_t rt, uint32_t rs, int16_t immediate,  int32_t result, int32_t rsSrc);
		void logBranch(std::string mnemonic, uint32_t rt, uint32_t rs, int16_t offset, bool jump, uint32_t targetAddress, int32_t rsSrc, int32_t rtSrc, bool compareToZero);
		void logDelayBranch();
		void logMove(std::string mnemonic, uint32_t rt, uint32_t rd, int32_t rdSrc);
	};
};
