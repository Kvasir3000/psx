#pragma once 

#include <stdint.h>
#define SIZE  120000

namespace psx 
{
	class BUS
	{
	private:
		uint8_t m_memory[SIZE] = { 0 };
		struct InstructionDescriptor
		{
			int16_t  immediate;
			int16_t  offset;
			uint16_t rt : 5;
			uint16_t regimmType : 5;
			uint16_t rs : 5;
			uint16_t primaryOpcode : 6; 
			uint16_t secondaryOpcode : 6;
			uint16_t rd : 5;
		};

		void addInstruction(InstructionDescriptor instruction);
		void addNOP();
		void endProgram();
		int32_t instructionCounter = 0;
	public:
		BUS();
	    ~BUS() {}; 
		uint8_t  readByte(uint32_t memoryAddress);
		uint32_t readWord(uint32_t memoryAddress);
		int32_t  getInstructionCounter();
		bool     isProgramEnd(uint32_t pc);
	};
}
