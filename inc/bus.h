#pragma once 

#include <stdint.h>
#define SIZE  120000

namespace psx 
{
	class BUS
	{
	public:
		BUS();
		~BUS() {};
		uint8_t  readByte(uint32_t memoryAddress);
		uint16_t readHalfword(uint32_t memoryAddress);
		uint32_t readWord(uint32_t memoryAddress);

		void storeByte(uint32_t memoryAddress, uint8_t data);
		void storeHalfword(uint32_t memoryAddress, uint16_t data);
		void storeWord(uint32_t memoryAddress, uint16_t data);

		int32_t  getInstructionCounter();
		bool     isProgramEnd(uint32_t pc);

	private:
		uint8_t m_memory[SIZE] = { 0 };
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
		
		void initRegister(uint32_t regiseterIdx, uint32_t value);
		void mlt(uint32_t rs, uint32_t rt);
		void mltu(uint32_t rs, uint32_t rt);
		void nor(uint32_t rd, uint32_t rs, uint32_t rt);
		void or(uint32_t rd, uint32_t rs, uint32_t rt);
		void ori(uint32_t rt, uint32_t rs, uint16_t immediate);
		void addi(uint32_t rt, uint32_t rs, int16_t immediate);
		void add(uint32_t rs, uint32_t rt, int32_t rd);
		void sb(uint32_t base, uint32_t rt, uint16_t offset);
		void sh(uint32_t base, uint32_t rt, uint16_t offset);
		void lb(uint32_t rt, uint32_t base, int16_t offset);
		void lh(uint32_t rt, uint32_t base, int16_t offset);
		void lw(uint32_t rt, uint32_t base, int16_t offset);
		void sll(uint32_t rd, uint32_t rt, uint32_t sa);
		void sllv(uint32_t rd, uint32_t rt, uint32_t rs);
		void slt(uint32_t rd, uint32_t rs, uint32_t rt);

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
