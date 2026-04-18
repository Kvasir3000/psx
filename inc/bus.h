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
