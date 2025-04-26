#pragma once

#include <fstream>
#include <stdint.h>
#include <string>

#include "../inc/debugger_constants.h"

namespace psx
{
	class Debugger
	{
	private:
		std::ofstream m_traceFile;
		uint32_t      m_pc;
	public:
		Debugger();
		~Debugger();

		void logWarning(std::string message);

		void setPC(uint32_t pc);
		void logRegisterTypeArithmetic(std::string mnemomic, uint32_t rd, uint32_t rs, uint32_t rt, int32_t result, int32_t rsSrc, int32_t rtSrc);
		void logImmediateTypeArithmetic(std::string mnemomic, uint32_t rt, uint32_t rs, int16_t immediate,  int32_t result, int32_t rsSrc);
		void logJump(std::string mnemonic, uint32_t rt, uint32_t rs, int16_t offset, bool jump, uint32_t targetAddress, int32_t rsSrc, int32_t rtSrc);
		void logDelayJump();
	};
};
