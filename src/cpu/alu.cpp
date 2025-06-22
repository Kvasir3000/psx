#include <assert.h>

#include "../../inc/cpu/cpu.h"



void mips::CPU::fillPrimaryOpcodeTable()
{
	m_primaryOpcodeTable[ADDI]  = std::bind(&CPU::addi,  this);
	m_primaryOpcodeTable[ADDIU] = std::bind(&CPU::addiu, this);
	m_primaryOpcodeTable[ANDI]  = std::bind(&CPU::andi,  this);
	m_primaryOpcodeTable[BEQ]   = std::bind(&CPU::beq,   this);
	m_primaryOpcodeTable[BNE]   = std::bind(&CPU::bne,   this);
	m_primaryOpcodeTable[COP0]  = std::bind(&CPU::cop,   this);
	m_primaryOpcodeTable[COP2]  = std::bind(&CPU::cop,   this);
	m_primaryOpcodeTable[J]     = std::bind(&CPU::jump,  this);
	m_primaryOpcodeTable[JAL]   = std::bind(&CPU::jal,   this);
	m_primaryOpcodeTable[LB]    = std::bind(&CPU::lb ,   this);
	m_primaryOpcodeTable[LBU]   = std::bind(&CPU::lbu,   this);
	m_primaryOpcodeTable[LH]    = std::bind(&CPU::lh,    this);
	m_primaryOpcodeTable[LHU]   = std::bind(&CPU::lhu,   this);
	m_primaryOpcodeTable[LUI]   = std::bind(&CPU::lui,   this);
	m_primaryOpcodeTable[LW]    = std::bind(&CPU::lw,    this);

}

void mips::CPU::fillSecondaryOpcodeTable()
{
	m_secondaryOpcodeTable[ADD]   = std::bind(&CPU::add,    this);
	m_secondaryOpcodeTable[ADDU]  = std::bind(&CPU::addu,   this);
	m_secondaryOpcodeTable[AND]   = std::bind(&CPU::and_,   this);
	m_secondaryOpcodeTable[BREAK] = std::bind(&CPU::break_, this);
	m_secondaryOpcodeTable[DIV]   = std::bind(&CPU::div,    this);
	m_secondaryOpcodeTable[DIVU]  = std::bind(&CPU::divu,   this);
	m_secondaryOpcodeTable[JALR]  = std::bind(&CPU::jalr,   this);
	m_secondaryOpcodeTable[JR]    = std::bind(&CPU::jr,     this);
}

void mips::CPU::fillREGIMMOpcodeTable()
{
	m_regimmOpcodeTable[BGEZ]   = std::bind(&CPU::bgez,   this);
	m_regimmOpcodeTable[BGEZAL] = std::bind(&CPU::bgezal, this);
	m_regimmOpcodeTable[BGTZ]   = std::bind(&CPU::bgtz,   this);
	m_regimmOpcodeTable[BLEZ]   = std::bind(&CPU::blez,   this);
	m_regimmOpcodeTable[BLTZ]   = std::bind(&CPU::bltz,   this);
	m_regimmOpcodeTable[BLTZAL] = std::bind(&CPU::bltzal, this);
}

void mips::CPU::fillCOPOpcodeTable()
{
	m_copOpcodeTable[CFC] = std::bind(&CPU::cfc2, this);
	m_copOpcodeTable[CTC] = std::bind(&CPU::ctc2, this);
}

void mips::CPU::executeRegisterTypeArithmeticOp(std::string mnemonic, std::function<int32_t(uint32_t, uint32_t)> arithmeticOp, ArithmeticOpFlags opFlags)
{
	uint32_t rd = m_instruction.getRD();
	uint32_t rs = m_instruction.getRS();
	uint32_t rt = m_instruction.getRT();

	int32_t result = arithmeticOp(m_registerFile[rs], m_registerFile[rt]);

	if (m_context->isDebug())
	{
		if (!opFlags.isMultiplicative)
		{
			m_context->getDebugger()->logRegisterTypeArithmetic(mnemonic, rd, rs, rt, result, m_registerFile[rs], m_registerFile[rt], opFlags.isSigned);
		}
		else
		{
			m_context->getDebugger()->logRegisterTypeMultiplicativeArithmetic(mnemonic, rs, rt, m_hi, m_lo, m_registerFile[rs], m_registerFile[rt], opFlags.isSigned);
		}
	}

	if (opFlags.catchException && checkOverflow(m_registerFile[rs], m_registerFile[rt], result))
	{
		raiseException("Integer overflow");
	}

	if (!opFlags.isMultiplicative) // In case of mul/div results are stored from lambda function
	{
		m_registerFile[rd] = static_cast<uint32_t>(result);
	}
}


void mips::CPU::executeImmediateTypeArithmeticOp(std::string mnemonic, std::function<int32_t(uint32_t, uint16_t)> arithmeticOpcode, bool catchException)
{
	uint32_t rs = m_instruction.getRS();
	uint32_t rt = m_instruction.getRT();
	int16_t immediate = m_instruction.getImmediate();

	int32_t result = arithmeticOpcode(m_registerFile[rs], immediate);

	if (m_context->isDebug())
	{
		m_context->getDebugger()->logImmediateTypeArithmetic(mnemonic, rt, rs, immediate, result, m_registerFile[rs]);
	}

	if (catchException && checkOverflow(m_registerFile[rs], immediate, result))
	{
		raiseException("Integer overflow");
	}

	m_registerFile[rt] = static_cast<uint32_t>(result);
}

void mips::CPU::executeBranchOp(std::string mnemonic, std::function<bool(uint32_t, uint32_t)> branchCondition, bool compareToZero)
{
	uint32_t rs = m_instruction.getRS();
	uint32_t rt = m_instruction.getRT();
	int16_t  offset = m_instruction.getImmediate();

	if (branchCondition(m_registerFile[rs], m_registerFile[rt]))
	{
		m_delaySlot.status = cpu_constants::DelaySlotState::Pending;
		m_delaySlot.targetAddress = m_pc + cpu_constants::WORD_SIZE + (offset << 2);
	}
	
	if (m_context->isDebug())
	{
		m_context->getDebugger()->logBranch(mnemonic,
			                                rt,
			                                rs,
			                                offset,
			                                m_delaySlot.status == cpu_constants::DelaySlotState::Pending,
			                                m_delaySlot.targetAddress,
			                                m_registerFile[rs],
			                                m_registerFile[rt],
			                                compareToZero);
	}
}

void mips::CPU::executeJumpOp(std::string mnemonic)
{
	uint32_t target = m_instruction.getJumpTarget();
	m_delaySlot.status = cpu_constants::DelaySlotState::Pending;
	m_delaySlot.targetAddress = ((m_pc + cpu_constants::WORD_SIZE) & cpu_constants::PC_HIGH_BITS_MASK) | (target << 2);

	if (m_context->isDebug())
	{
		m_context->getDebugger()->logJump("j", m_delaySlot.targetAddress);
	}
}

void mips::CPU::excecuteJumpRegisterOp(std::string mnemonic)
{
	uint32_t rd = m_instruction.getRD();
	uint32_t rs = m_instruction.getRS();
	
	if (rd) // if rd is 0, means that this is not link instruction
	{
		assert(m_instruction.getSecondaryOpcode() == JALR);
		m_registerFile[rd] = m_pc + (2 * cpu_constants::WORD_SIZE);
	}
	m_delaySlot.status = cpu_constants::DelaySlotState::Pending; 
	m_delaySlot.targetAddress = m_registerFile[rs];

	if (m_context->isDebug())
	{
		m_context->getDebugger()->logJumpRegister(mnemonic, rs, m_registerFile[rs]);
	}
}

void mips::CPU::executeLoadOp(std::string mnemonic, std::function<uint32_t(uint32_t)> loadOp, LoadOpFlags opFlags)
{
	uint32_t rt = m_instruction.getRT();
	uint32_t base = m_instruction.getRS();
	int32_t  offset = m_instruction.getOffset();

	uint32_t address = m_registerFile[base] + offset;

	DelayLoad load;
	load.status = cpu_constants::DelaySlotState::Pending;
	load.data = loadOp(address);
	load.registerIdx = rt;
	load.loadType = opFlags.loadType;
	load.sign = opFlags.isSigned; 

	m_delayLoads.emplace(load);

	if (m_context->isDebug())
	{
		m_context->getDebugger()->logLoad(mnemonic, rt, offset, base, m_registerFile[base]);
	}
}


void mips::CPU::add()
{
	auto addOp = [](int32_t operand1, int32_t operand2)->int32_t { return operand1 + operand2; };
	ArithmeticOpFlags opFlags = { true, false, true };
	executeRegisterTypeArithmeticOp("add", addOp, opFlags);
} 

void mips::CPU::addi()
{
	auto addiOp = [](int32_t operand1, int16_t operand2)->int32_t { return operand1 + operand2; };
	executeImmediateTypeArithmeticOp("addi", addiOp, true);
}

void mips::CPU::addiu()
{
	auto addiOp = [](uint32_t operand1, int16_t operand2)->int32_t { return operand1 + operand2; };
	executeImmediateTypeArithmeticOp("addiu", addiOp, false);
}

void mips::CPU::addu()
{
	auto addOp = [](int32_t operand1, int32_t operand2)->int32_t { return operand1 + operand2; };
	ArithmeticOpFlags opFlags = { false, false, true };
	executeRegisterTypeArithmeticOp("addu", addOp, opFlags);
}

void mips::CPU::and_()
{
	auto andOp = [](uint32_t operand1, uint32_t operand2)->int32_t { return operand1 & operand2; };
	ArithmeticOpFlags opFlags = { false, false, false };
	executeRegisterTypeArithmeticOp("and", andOp, opFlags);
}

void mips::CPU::andi()
{
	auto andiOp = [](uint32_t operand1, uint16_t operand2)->int32_t { return operand1 & operand2; };
	executeImmediateTypeArithmeticOp("andi", andiOp, true);
}

void mips::CPU::beq()
{
	auto equals = [](int32_t operand1, int32_t operand2)->bool { return operand1 == operand2; };
	executeBranchOp("beq", equals, false);
}

void mips::CPU::bgez()
{
	auto greaterEqualToZero = [](int32_t operand1, uint32_t operand2)->bool { return operand1 >= 0; };
	executeBranchOp("bgez", greaterEqualToZero, true);
}

void mips::CPU::bgezal()
{
	m_registerFile[cpu_constants::LINK_REGISTER] = m_pc + (cpu_constants::WORD_SIZE * 2);
	auto greaterEqualToZero = [](int32_t operand1, uint32_t operand2)->bool { return operand1 >= 0; };
	executeBranchOp("bgezal", greaterEqualToZero, true);
}

void mips::CPU::bgtz()
{
	auto greaterThanZero = [](int32_t operand1, uint32_t operand2)->bool { return operand1 > 0;  };
	executeBranchOp("bgtz", greaterThanZero, true);
}

void mips::CPU::blez()
{
	auto lessEqualToZero = [](int32_t operand1, uint32_t operand2)->bool { return operand1 <= 0;  };
	executeBranchOp("blez", lessEqualToZero, true);
}

void mips::CPU::bltz()
{
	auto lessThanZero = [](int32_t operand1, uint32_t operand2)->bool { return operand1 < 0; };
	executeBranchOp("bltz", lessThanZero, true);
}

void mips::CPU::bltzal()
{
	m_registerFile[cpu_constants::LINK_REGISTER] = m_pc + (cpu_constants::WORD_SIZE * 2);
	auto lessThanZero = [](int32_t operand1, uint32_t operand2)->bool { return operand1 < 0; };
	executeBranchOp("bltzal", lessThanZero, true);
}

void mips::CPU::bne()
{
	auto notEqual = [](uint32_t operand1, uint32_t operand2)->bool { return operand1 != operand2; };
	executeBranchOp("bne", notEqual, false);
}

void mips::CPU::break_()
{
	raiseException("Breakpoint");
}

void mips::CPU::cfc2()
{
	uint32_t rd = m_instruction.getRD();
	uint32_t rt = m_instruction.getRT();
	m_registerFile[rt] = m_gte.readControlRegister(rd);

	if (m_context->isDebug())
	{
		m_context->getDebugger()->logMove("cfc2", rt, rd, m_registerFile[rt]);
	}
	raiseException("Coprocessor unusable");
}

void mips::CPU::cop()
{
	uint32_t    copIdx = m_instruction.getCOPIdx();
	std::string warning = "COP" + std::to_string(copIdx) + " is note implemented yet";
	m_context->getDebugger()->logWarning(warning);
}

void mips::CPU::ctc2()
{
	uint32_t rd = m_instruction.getRD();
	uint32_t rt = m_instruction.getRT();
	m_gte.writeContolRegister(rd, m_registerFile[rt]);
	
	if (m_context->isDebug())
	{
		m_context->getDebugger()->logMove("ctc2", rd, rt, m_registerFile[rt]);
	}

	raiseException("Coprocessor unusable");
}

// Modulo looks might be working wrong, need to check
void mips::CPU::div()
{
	auto divideOp = [this](int32_t operand1, int32_t operand2)->uint32_t
    {
			if (operand2 != 0)
			{
				m_lo = operand1 / operand2;
				m_hi = operand1 % operand2;
			}
			else
			{
				m_context->getDebugger()->logWarning("Instruction tried to divide by 0: ");
			}
			return 0;
	};
	ArithmeticOpFlags opFlags = { false, true, true };
	executeRegisterTypeArithmeticOp("div", divideOp, opFlags);
}

void mips::CPU::divu()
{
	auto divideOp = [this](uint32_t operand1, uint32_t operand2)->uint32_t
	{
			if (operand2 != 0)
			{
				m_lo = operand1 / operand2;
				m_hi = operand1 % operand2;
			}
			else
			{
				m_context->getDebugger()->logWarning("Instruction tried to divide by 0: ");
			}
			return 0;
	};
	ArithmeticOpFlags opFlags = { false, true, false };
	executeRegisterTypeArithmeticOp("divu", divideOp, opFlags);
}

void mips::CPU::jump()
{
	executeJumpOp("j");
}

void mips::CPU::jal()
{
	m_registerFile[cpu_constants::LINK_REGISTER] = m_pc + (cpu_constants::WORD_SIZE * 2);
	executeJumpOp("jal");
}

void mips::CPU::jalr()
{
	excecuteJumpRegisterOp("jalr");
}

void mips::CPU::jr()
{
	excecuteJumpRegisterOp("jr");
}

void mips::CPU::lb()
{
	auto loadByteOp = [this](uint32_t memoryAddress)->int32_t { return m_context->getBus()->readByte(memoryAddress); };
	LoadOpFlags opFlags = {cpu_constants::DelayLoadType::Byte, true};
	executeLoadOp("lb", loadByteOp, opFlags);
}

void mips::CPU::lbu()
{
	auto loadByteOp = [this](uint32_t memoryAddress)->uint32_t { return m_context->getBus()->readByte(memoryAddress); };
	LoadOpFlags opFlags = { cpu_constants::DelayLoadType::Byte, false };
	executeLoadOp("lbu", loadByteOp, opFlags);
}

void mips::CPU::lh()
{
	auto loadHalfwordOp = [this](uint32_t memoryAddress)->int32_t { return m_context->getBus()->readHalfword(memoryAddress); };
	LoadOpFlags opFlags = { cpu_constants::DelayLoadType::Halfword, true };
	executeLoadOp("lh", loadHalfwordOp, opFlags);
};

void mips::CPU::lhu()
{
	auto loadHalfwordOp = [this](uint32_t memoryAddress)->uint32_t { return m_context->getBus()->readHalfword(memoryAddress); };
	LoadOpFlags opFlags = { cpu_constants::DelayLoadType::Halfword, false };
	executeLoadOp("lhu", loadHalfwordOp, opFlags);
}

void mips::CPU::lui()
{
	uint32_t rt = m_instruction.getRT();
	uint16_t immediate = m_instruction.getImmediate();
    
	m_registerFile[rt] = immediate << 16; 

	if (m_context->isDebug())
	{
		m_context->getDebugger()->logLoadUpperImmediate(rt, immediate, m_registerFile[rt]);
	}
}

void mips::CPU::lw()
{
	auto loadWord = [this](uint32_t memoryAddress)->uint32_t { return m_context->getBus()->readWord(memoryAddress); };
	LoadOpFlags opFlags = { cpu_constants::DelayLoadType::Word, false };
	executeLoadOp("lw", loadWord, opFlags);
}