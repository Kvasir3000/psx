#pragma once

#include <fstream>
#include <stdint.h>
#include <string>
#include <iostream>
#include <optional>

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
		void logBranch(const std::string& mnemonic, uint32_t rt, uint32_t rs, int16_t offset, bool jump, uint32_t targetAddress, int32_t rsSrc, int32_t rtSrc, bool compareToZero);
		void logJump(const std::string& mnemonic, uint32_t targetAddress);
		void logJumpRegister(const std::string& mnemonic, uint32_t rs, uint32_t rsSrc);
		void logMemoryOperation(const std::string& mnemonic, uint32_t rt, int32_t offset, uint32_t base, uint32_t baseSrc);
		void logLoadShift(uint32_t requestedMemory, uint32_t alignedMemory, uint32_t memoryData, uint32_t rt, uint32_t rtSrc, uint32_t rtResult);
		void logLoadUpperImmediate(uint32_t rt, uint32_t immediate, uint32_t result);
		void logDelayedBranch();
		void logDelayedLoad(uint32_t rt, uint32_t rtSrc, bool sign, bool byte, bool halfword);
		void logShiftLogical(const std::string& mnemonic, uint32_t rd, uint32_t rt, uint32_t shift, uint32_t rdSrc, uint32_t rtSrc);
		void logShiftVariable(const std::string& mnemonic, uint32_t rd, uint32_t rt, uint32_t rs, uint32_t rdSrc, uint32_t rtSrc, uint32_t rsSrc);
		
		template<uint32_t N>
		void logDecodedAssembly(const std::string& mnemonic, const std::array<std::string, N>& operands) const
		{
			OUTPUT_STREAM << mnemonic; 
			for (int i = 0; i < N; i++)
			{
				OUTPUT_STREAM << " " << operands[i];
				if (i < N - 1)
				{
					OUTPUT_STREAM << ",";
				}
			}
		};

		template<typename Value>
		void logDecodedValue(const Value& value, bool lastValue) const
		{
			OUTPUT_STREAM << std::hex << "0x" << value << std::dec << "(" << value << ")" << ((lastValue) ? "" : ", ");
		}

		template<typename ...Values>
		void logDecodedValues(const Values... values ) const
		{
			uint32_t idx = 0;
			([&]() { logDecodedValue(values, idx++ == (sizeof...(values) - 1)); }(), ...);
			OUTPUT_STREAM << "\n";
		}

		template<typename ...Operands>
		void logGenericRegOp(const std::string& mnemonic, Operands... ops) const
		{
			std::array<std::string, sizeof...(ops)> operands = { ops.name ... };
			logDecodedAssembly(mnemonic, operands);

			OUTPUT_STREAM << " // ";
			logDecodedValues(ops.value ...);
		}

		template<typename hi_type, typename lo_type, typename ...Operands>
		void logRegisterMultiplicativeArithmetic(const std::string& mnemonic, const hi_type& hi, 
			                                     const lo_type& lo, Operands... ops) const
		{
			std::array<std::string, sizeof...(ops)> operands = { ops.name ... }; 

			logDecodedAssembly(mnemonic, operands);
			OUTPUT_STREAM << " // hi = 0x" << std::hex << hi << "(" << std::dec << hi << ")";
			OUTPUT_STREAM << " lo = 0x" << std::hex << lo << "(" << std::dec << lo << "), ";
			logDecodedValues(ops.value ...);
		}
	};
};
