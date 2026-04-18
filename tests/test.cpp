#include "test.h"

namespace testing
{

	Test::Test(std::string name) : m_name(name)
	{

	}

	void Test::addRTBaseOffsetOp(uint32_t opcode, uint32_t rt, uint32_t base, int16_t offset)
	{
		InstructionDescriptor instruction = { 0 };
		instruction.primaryOpcode = opcode;
		instruction.base = base;
		instruction.offset = offset;
		instruction.rt = rt;
		addInstruction(instruction);
	}

	void Test::addRSRTOp(uint32_t opcode, uint32_t rs, uint32_t rt)
	{
		InstructionDescriptor instruction = { 0 };
		instruction.secondaryOpcode = opcode;
		instruction.rs = rs;
		instruction.rt = rt;
		addInstruction(instruction);
	}

	std::string Test::getName() 
	{
		return m_name; 
	};

	const std::uint8_t* Test::getTestBin()
	{
		return m_memory;
	}

	uint32_t Test::getInstructionCount()
	{
		return m_instructionCounter;
	}

	void Test::nop()
	{
		InstructionDescriptor instruction = { 0 };
		addInstruction(instruction);
	}

	void Test::endProgram()
	{
		InstructionDescriptor instruction = { 0 };
		instruction.immediate = cpu_constants::END;
		addInstruction(instruction);
	}

	void Test::addInstruction(InstructionDescriptor instruction)
	{
		assert(!(instruction.primaryOpcode && instruction.secondaryOpcode), "ERROR: Wrong instruction");
		///assert(!(instruction.immediate && instruction.rd), "ERROR: Wrong instruction");

		uint32_t memoryAddress = m_instructionCounter * cpu_constants::WORD_SIZE_BYTES;

		uint32_t instructionWord = 0;
		instructionWord |= instruction.secondaryOpcode;
		instructionWord |= static_cast<uint16_t>(instruction.immediate);
		instructionWord |= static_cast<uint16_t>(instruction.offset);
		instructionWord |= instruction.rd << cpu_constants::RD_OFFSET;
		instructionWord |= instruction.base << cpu_constants::RS_OFFSET;
		instructionWord |= instruction.rs << cpu_constants::RS_OFFSET;
		instructionWord |= instruction.sa << cpu_constants::SA_OFFSET;
		instructionWord |= instruction.rt << cpu_constants::RT_OFFSET;
		instructionWord |= instruction.regimmType << cpu_constants::REGIMM_TYPE_OFFSET;
		instructionWord |= instruction.copSuboperation << cpu_constants::COP_SUBOPERATION_OFFSET;
		instructionWord |= instruction.copIdx << cpu_constants::COP_INDEX_OFFSET;
		instructionWord |= instruction.copCofun ? cpu_constants::COP_OPERATION : 0;
		instructionWord |= instruction.copOp ? cpu_constants::COP_OPCODE : 0;
		instructionWord |= instruction.target;
		instructionWord |= instruction.primaryOpcode << cpu_constants::PRIMARY_OPCODE_OFFSET;

		memcpy(&m_memory[memoryAddress], &instructionWord, cpu_constants::WORD_SIZE_BYTES);

		m_instructionCounter++;
	}

	void Test::initRegister(uint32_t registerIdx, uint32_t value)
	{
		InstructionDescriptor instruction = { 0 };
		instruction.primaryOpcode = ADDI;
		instruction.rt = registerIdx;
		instruction.rs = registerIdx;
		instruction.immediate = value;
		addInstruction(instruction);
	}

	void Test::mlt(uint32_t rs, uint32_t rt)
	{
		addRSRTOp(MULT, rs, rt);
	}

	void Test::mltu(uint32_t rs, uint32_t rt)
	{
		addRSRTOp(MULTU, rs, rt);
	}

	void Test::nor(uint32_t rd, uint32_t rs, uint32_t rt)
	{
		InstructionDescriptor instruction = { 0 };
		instruction.secondaryOpcode = NOR;
		instruction.rd = rd;
		instruction.rs = rs;
		instruction.rt = rt;
		addInstruction(instruction);
	}

	void Test::_or(uint32_t rd, uint32_t rs, uint32_t rt)
	{
		InstructionDescriptor instruction = { 0 };
		instruction.secondaryOpcode = OR;
		instruction.rd = rd;
		instruction.rs = rs;
		instruction.rt = rt;
		addInstruction(instruction);
	}

	void Test::ori(uint32_t rt, uint32_t rs, uint16_t immediate)
	{
		InstructionDescriptor instruction = { 0 };
		instruction.primaryOpcode = ORI;
		instruction.rt = rt;
		instruction.rs = rs;
		instruction.immediate = immediate;
		addInstruction(instruction);
	}

	void Test::addi(uint32_t rt, uint32_t rs, int16_t immediate)
	{
		InstructionDescriptor instruction = { 0 };
		instruction.primaryOpcode = ADDI;
		instruction.rt = rt;
		instruction.rs = rs;
		instruction.immediate = immediate;
		addInstruction(instruction);
	}

	void Test::div(uint32_t rs, uint32_t rt)
	{
		addRSRTOp(DIV, rs, rt);
	}

	void Test::add(uint32_t rs, uint32_t rt, int32_t rd)
	{
		InstructionDescriptor instruction = { 0 };
		instruction.secondaryOpcode = ADD;
		instruction.rt = rs;
		instruction.rs = rt;
		instruction.rd = rd;
		addInstruction(instruction);
	}

	void Test::sb(uint32_t base, uint32_t rt, uint16_t offset)
	{
		InstructionDescriptor instruction = { 0 };
		instruction.primaryOpcode = SB;
		instruction.base = base;
		instruction.rt = rt;
		instruction.offset = offset;
		addInstruction(instruction);
	}

	void Test::sh(uint32_t base, uint32_t rt, uint16_t offset)
	{
		InstructionDescriptor instruction = { 0 };
		instruction.primaryOpcode = SH;
		instruction.base = base;
		instruction.rt = rt;
		instruction.offset = offset;
		addInstruction(instruction);
	}


	void Test::lb(uint32_t rt, uint32_t base, int16_t offset)
	{
		addRTBaseOffsetOp(LB, rt, base, offset);
	}

	void Test::lbu(uint32_t rt, uint32_t base, int16_t offset)
	{
		addRTBaseOffsetOp(LBU, rt, base, offset);
	}
	
	void Test::lh(uint32_t rt, uint32_t base, int16_t offset)
	{
		addRTBaseOffsetOp(LH, rt, base, offset);
	}

	void Test::lw(uint32_t rt, uint32_t base, int16_t offset)
	{
		addRTBaseOffsetOp(LW, rt, base, offset);
	}
	
	void Test::lwl(uint32_t rt, uint32_t base, int16_t offset)
	{
		addRTBaseOffsetOp(LWL, rt, base, offset);
	}

	void Test::lwr(uint32_t rt, uint32_t base, int16_t offset)
	{
		addRTBaseOffsetOp(LWR, rt, base, offset);
	}
	
	void Test::lwc2(uint32_t rt, uint32_t base, int16_t offset)
	{
		addRTBaseOffsetOp(LWC2, rt, base, offset);
	}

	void Test::sll(uint32_t rd, uint32_t rt, uint32_t sa)
	{
		InstructionDescriptor instruction = { 0 };
		instruction.secondaryOpcode = SLL;
		instruction.rd = rd;
		instruction.rt = rt;
		instruction.sa = sa;
		addInstruction(instruction);
	}

	void Test::sllv(uint32_t rd, uint32_t rt, uint32_t rs)
	{
		InstructionDescriptor instruction = { 0 };
		instruction.secondaryOpcode = SLLV;
		instruction.rd = rd;
		instruction.rt = rt;
		instruction.rs = rs;
		addInstruction(instruction);
	}

	void Test::slt(uint32_t rd, uint32_t rs, uint32_t rt)
	{
		InstructionDescriptor instruction = { 0 };
		instruction.secondaryOpcode = SLT;
		instruction.rd = rd;
		instruction.rs = rs;
		instruction.rt = rt;
		addInstruction(instruction);
	}

	void Test::mfc(uint32_t rt, uint32_t rd, uint32_t copIdx)
	{
		InstructionDescriptor instruction = { 0 };
		instruction.copSuboperation = MFC;
		instruction.copIdx = copIdx;
		instruction.rt = rt;
		instruction.rd = rd;
		instruction.copOp = true;
		addInstruction(instruction);
	}

	void Test::mtc(uint32_t rt, uint32_t rd, uint32_t copIdx)
	{
		InstructionDescriptor instruction = { 0 };
		instruction.copOp = true;
		instruction.copSuboperation = MTC;
		instruction.copIdx = copIdx;
		instruction.rt = rt;
		instruction.rd = rd;
		addInstruction(instruction);
	}

	void Test::mthi(uint32_t rs)
	{
		InstructionDescriptor instruction = { 0 };
		instruction.secondaryOpcode = MTHI;
		instruction.rs = rs;
		addInstruction(instruction);
	};

	void Test::mtlo(uint32_t rs)
	{
		InstructionDescriptor instruction = { 0 };
		instruction.secondaryOpcode = MTLO;
		instruction.rs = rs;
		addInstruction(instruction);
	};

	void Test::mfhi(uint32_t rd)
	{
		InstructionDescriptor instruction = { 0 };
		instruction.secondaryOpcode = MFHI;
		instruction.rd = rd;
		addInstruction(instruction);
	};

	void Test::mflo(uint32_t rd)
	{
		InstructionDescriptor instruction = { 0 };
		instruction.secondaryOpcode = MFLO;
		instruction.rd = rd;
		addInstruction(instruction);
	};

	void Test::ctc2(uint32_t rt, uint32_t rd)
	{
		InstructionDescriptor instruction = { 0 };
		instruction.copOp = true;
		instruction.copSuboperation = CTC;
		instruction.rd = rd;
		instruction.rt = rt;
		instruction.copIdx = 1;
		addInstruction(instruction);
	};

	void Test::cfc2(uint32_t rt, uint32_t rd)
	{
		InstructionDescriptor instruction = { 0 };
		instruction.copOp = true;
		instruction.copSuboperation = CFC;
		instruction.rd = rd;
		instruction.rt = rt;
		instruction.copIdx = 1;
		addInstruction(instruction);
	};
}