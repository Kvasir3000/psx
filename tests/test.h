#pragma once

#include <stdint.h>
#include <assert.h>
#include <unordered_map>
#include <memory>
#include <string>
#include "constants/cpu_constants.h"
#include "cpu/opcodes.h"

namespace testing
{ 
class Test
{
private: 
	static constexpr uint32_t				   MEMORY_SIZE = 500;
	std::string								   m_name; 
	std::unordered_map<std::string, uint32_t>  m_labelAddrMap;

	struct RelocationEntry 
	{
		std::string name; 
		uint32_t    op;
		uint32_t    rs; 
		uint32_t    rt;
	};
	std::unordered_map<uint32_t, RelocationEntry>  m_primaryRelocationEntries;
	std::unordered_map<uint32_t, RelocationEntry>  m_regimmRelocationEntries;
	void addRTBaseOffsetOp(uint32_t opcode, uint32_t rt, uint32_t base, int16_t offset);
	void addRSRTOp(uint32_t opcode, uint32_t rs, uint32_t rt);
public:
	Test(std::string name);

	std::string			getName();
	const std::uint8_t* getTestBin();
	uint32_t            getStartAddr();
	uint32_t			getInstructionCount();
	void                link();


	uint32_t    m_instructionCounter = 0;
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
		bool     regimm;
	};
	
	uint32_t getCurrentAddr();

	void addInstruction(InstructionDescriptor instruction, uint32_t memoryAddr = cpu_constants::END);
	void nop();
	void label(std::string name);
	void endProgram();

	void initRegister(uint32_t regiseterIdx, uint32_t value);
	void mlt(uint32_t rs, uint32_t rt);
	void mltu(uint32_t rs, uint32_t rt);
	void nor(uint32_t rd, uint32_t rs, uint32_t rt);
	void or_ (uint32_t rd, uint32_t rs, uint32_t rt);
	void ori(uint32_t rt, uint32_t rs, uint16_t immediate);
	void addi(uint32_t rt, uint32_t rs, int16_t immediate);
	void div(uint32_t rs, uint32_t rt);
	void add(int32_t rd, uint32_t rs, uint32_t rt);
	void sb(uint32_t base, uint32_t rt, uint16_t offset);
	void sh(uint32_t base, uint32_t rt, uint16_t offset);
	void lb(uint32_t rt, uint32_t base, int16_t offset);
	void lbu(uint32_t rt, uint32_t base, int16_t offset);
	void lh(uint32_t rt, uint32_t base, int16_t offset);
	void lw(uint32_t rt, uint32_t base, int16_t offset);
	void lwl(uint32_t rt, uint32_t base, int16_t offset);
	void lwr(uint32_t rt, uint32_t base, int16_t offset);
	void lwc2(uint32_t rt, uint32_t base, int16_t offset);
	void sll(uint32_t rd, uint32_t rt, uint32_t sa);
	void sllv(uint32_t rd, uint32_t rt, uint32_t rs);
	void slt(uint32_t rd, uint32_t rs, uint32_t rt);
	void mfc(uint32_t rt, uint32_t rd, uint32_t copIdx);
	void mfc0(uint32_t rt, uint32_t rd);
	void mfc2(uint32_t rt, uint32_t rd);
	void mtc(uint32_t rt, uint32_t rd, uint32_t copIdx);
	void mtc0(uint32_t rt, uint32_t rd);
	void mtc2(uint32_t rt, uint32_t rd);
	void mthi(uint32_t rs);
	void mtlo(uint32_t rs);
	void mfhi(uint32_t rd);
	void mflo(uint32_t rd);
	void ctc2(uint32_t rt, uint32_t rd);
	void cfc2(uint32_t rt, uint32_t rd);
	void beq(uint32_t rs, uint32_t rt, const std::string& label);
	void beq(uint32_t rs, uint32_t rt, int16_t offset, uint32_t relocationAddr = cpu_constants::END);
	void bgez(uint32_t rs, const std::string& label);
	void bgez(uint32_t rs, int16_t offset, uint32_t relocationAddr = cpu_constants::END);
	void bgezal(uint32_t rs, const std::string& label);
	void bgezal(uint32_t rs, int16_t offset, uint32_t relocationAddr = cpu_constants::END);
	void bgtz(uint32_t rs, const std::string& label);
	void bgtz(uint32_t rs, int16_t offset, uint32_t relocationAddr = cpu_constants::END);
	void blez(uint32_t rs, const std::string& label);
	void blez(uint32_t rs, int16_t offset, uint32_t relocationAddr = cpu_constants::END);
	void bltz(uint32_t rs, const std::string& label);
	void bltz(uint32_t rs, int16_t offset, uint32_t relocationAddr = cpu_constants::END);
	void bltzal(uint32_t rs, const std::string& label);
	void bltzal(uint32_t rs, int16_t offset, uint32_t relocationAddr = cpu_constants::END);
	void bne(uint32_t rs, uint32_t rt, const std::string& label);
	void bne(uint32_t rs, uint32_t rt, int16_t offset, uint32_t relocationAddr = cpu_constants::END);
	void j(uint32_t target, uint32_t relocationAddr = cpu_constants::END);
	void j(const std::string& name);
	void jal(uint32_t target, uint32_t relocationAddr = cpu_constants::END);
	void jal(const std::string& procedureName);
	void jalr(uint32_t rs, uint32_t rd);
	void jr(uint32_t rs);
};
}