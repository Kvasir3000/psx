#pragma once

#include <stdint.h>
#include <assert.h>
#include <memory>
#include <string>
#include "constants/cpu_constants.h"
#include "cpu/opcodes.h"

namespace testing
{ 
class Test
{
private: 
	static constexpr uint32_t MEMORY_SIZE = 300;
	std::string m_name; 

	void addRTBaseOffsetOp(uint32_t opcode, uint32_t rt, uint32_t base, int16_t offset);
public:
	Test(std::string name);

	std::string			getName();
	const std::uint8_t* getTestBin();
	uint32_t			getInstructionCount();

 protected: 
	uint8_t   m_memory[MEMORY_SIZE] = { 0 };
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
	void nop();
	void endProgram();

	void initRegister(uint32_t regiseterIdx, uint32_t value);
	void mlt(uint32_t rs, uint32_t rt);
	void mltu(uint32_t rs, uint32_t rt);
	void nor(uint32_t rd, uint32_t rs, uint32_t rt);
	void _or (uint32_t rd, uint32_t rs, uint32_t rt);
	void ori(uint32_t rt, uint32_t rs, uint16_t immediate);
	void addi(uint32_t rt, uint32_t rs, int16_t immediate);
	void div(uint32_t rs, uint32_t rt);
	void add(uint32_t rs, uint32_t rt, int32_t rd);
	void sb(uint32_t base, uint32_t rt, uint16_t offset);
	void sh(uint32_t base, uint32_t rt, uint16_t offset);
	void lb(uint32_t rt, uint32_t base, int16_t offset);
	void lh(uint32_t rt, uint32_t base, int16_t offset);
	void lw(uint32_t rt, uint32_t base, int16_t offset);
	void lwl(uint32_t rt, uint32_t base, int16_t offset);
	void lwr(uint32_t rt, uint32_t base, int16_t offset);
	void sll(uint32_t rd, uint32_t rt, uint32_t sa);
	void sllv(uint32_t rd, uint32_t rt, uint32_t rs);
	void slt(uint32_t rd, uint32_t rs, uint32_t rt);
	void mfc(uint32_t rt, uint32_t rd, uint32_t copIdx);
	void mtc(uint32_t rt, uint32_t rd, uint32_t copIdx);
	void mthi(uint32_t rs);
	void mtlo(uint32_t rs);
	void mfhi(uint32_t rd);
	void mflo(uint32_t rd);
	void ctc2(uint32_t rt, uint32_t rd);
	void cfc2(uint32_t rt, uint32_t rd);

	int32_t m_instructionCounter = 0;
};
}