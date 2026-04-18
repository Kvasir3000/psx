#pragma once 

#include <stdint.h>
#include "../tests/testbuilder.h"

namespace psx 
{
	class BUS
	{
	public:
		BUS();
		~BUS() {};
		void     loadTest(testing::Test& test);
		uint8_t  readByte(uint32_t memoryAddress);
		uint16_t readHalfword(uint32_t memoryAddress);
		uint32_t readWord(uint32_t memoryAddress);

		void storeByte(uint32_t memoryAddress, uint8_t data);
		void storeHalfword(uint32_t memoryAddress, uint16_t data);
		void storeWord(uint32_t memoryAddress, uint16_t data);

		int32_t  getInstructionCounter();
		bool     isProgramEnd(uint32_t pc);

		void     reset();

	private:

		static constexpr uint32_t MEMORY_SIZE = 300;
		uint8_t m_memory[MEMORY_SIZE] = { 0 };
		struct InstructionDescriptor
		{
			uint8_t  sa; 
			int16_t  immediate;
			int16_t  offset;
			uint16_t rt : 5;
			uint16_t regimmType : 5;
			uint16_t rs : 5;
			uint16_t copSuboperation;
			uint16_t copIdx;
			uint16_t primaryOpcode : 6;
			uint16_t secondaryOpcode : 6;
			uint16_t rd : 5;
			uint16_t base : 5;
			uint32_t target : 28;

			bool     copCofun;
			bool     copOp;
		};

		void addInstruction(InstructionDescriptor instruction);
		void addNOP();
		void endProgram();
	

		// Testing programs
		void testLoadOperations();
		void testLoadCOP2();
		void testLoadWLR();
		void testMove();
		void testArithmeticOperations();
		void testStore();
		void testSetOn();
		int32_t instructionCounter = 0;
	};
}
