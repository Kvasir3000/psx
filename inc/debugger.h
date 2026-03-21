#pragma once

#include <fstream>
#include <stdint.h>
#include <string>

#include "../inc/constants/debugger_constants.h"

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
		void logPC(uint32_t pc);
		void logRegisterTypeArithmetic(const std::string& mnemonic, uint32_t rd, uint32_t rs, uint32_t rt, uint32_t result, uint32_t rsSrc, uint32_t rtSrc, bool sign);
		void logRegisterTypeMultiplicativeArithmetic(const std::string& mnemonic, uint32_t rs, uint32_t rt, uint32_t hi, uint32_t lo, uint32_t rsSrc, uint32_t rtSrc, bool sign);
		void logImmediateTypeArithmetic(const std::string& mnemonic, uint32_t rt, uint32_t rs, int16_t immediate,  int32_t result, int32_t rsSrc);
		void logBranch(const std::string& mnemonic, uint32_t rt, uint32_t rs, int16_t offset, bool jump, uint32_t targetAddress, int32_t rsSrc, int32_t rtSrc, bool compareToZero);
		void logJump(const std::string& mnemonic, uint32_t targetAddress);
		void logJumpRegister(const std::string& mnemonic, uint32_t rs, uint32_t rsSrc);
		void logMemoryOperation(const std::string& mnemonic, uint32_t rt, int32_t offset, uint32_t base, uint32_t baseSrc);
		void logLoadShift(uint32_t requestedMemory, uint32_t alignedMemory, uint32_t memoryData, uint32_t rt, uint32_t rtSrc, uint32_t rtResult);
		void logLoadUpperImmediate(uint32_t rt, uint32_t immediate, uint32_t result);
		void logDelayedBranch();
		void logDelayedLoad(uint32_t rt, uint32_t rtSrc, bool sign, bool byte, bool halfword);
		void logMove(const std::string& mnemonic, uint32_t rt, uint32_t rd, int32_t rdSrc);
		void logMoveToHiLo(const std::string& mnemonic, uint32_t dstRegister, uint32_t rs, uint32_t rsSrc);
		void logShiftLogical(const std::string& mnemonic, uint32_t rd, uint32_t rt, uint32_t shift, uint32_t rdSrc, uint32_t rtSrc);
		void logShiftVariable(const std::string& mnemonic, uint32_t rd, uint32_t rt, uint32_t rs, uint32_t rdSrc, uint32_t rtSrc, uint32_t rsSrc);
		void logRegisterSetOn(const std::string& mnemonic, uint32_t rd, uint32_t rt, uint32_t rs, uint32_t rdSrc, uint32_t rtSrc, uint32_t rsSrc);
	};
};
