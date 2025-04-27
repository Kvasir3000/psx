#include "../../inc/cpu.h"


void mips::CPU::fillPrimaryOpcodeTable()
{



	m_primaryOpcodeTable[ADDI]  = std::bind(&CPU::addi,  this);
	m_primaryOpcodeTable[ADDIU] = std::bind(&CPU::addiu, this);
	m_primaryOpcodeTable[ANDI]  = std::bind(&CPU::andi,  this);
	m_primaryOpcodeTable[BEQ]   = std::bind(&CPU::beq,   this);
	m_primaryOpcodeTable[BNE]   = std::bind(&CPU::bne,   this);
}

void mips::CPU::fillSecondaryOpcodeTable()
{
	m_secondaryOpcodeTable[ADD]   = std::bind(&CPU::add,    this);
	m_secondaryOpcodeTable[ADDU]  = std::bind(&CPU::addu,   this);
	m_secondaryOpcodeTable[AND]   = std::bind(&CPU::and_,   this);
	m_secondaryOpcodeTable[BREAK] = std::bind(&CPU::break_, this);
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

void mips::CPU::executeRegisterTypeArithmeticOp(std::string mnemonic, std::function<int32_t(uint32_t, uint32_t)> arithmeticOp, bool catchException)
{
	uint32_t rd = m_instruction.getRD();
	uint32_t rs = m_instruction.getRS();
	uint32_t rt = m_instruction.getRT();

	int32_t result = arithmeticOp(m_registerFile[rs], m_registerFile[rt]);

	if (m_context->isDebug())
	{
		m_context->getDebugger()->logRegisterTypeArithmetic(mnemonic, rd, rs, rt, result, m_registerFile[rs], m_registerFile[rt]);
	}

	if (catchException && checkOverflow(m_registerFile[rs], m_registerFile[rt], result))
	{
		raiseException("IntegerOverflow");
	}

	m_registerFile[rd] = static_cast<uint32_t>(result);
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
		raiseException("IntegerOverflow");
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

void mips::CPU::add()
{
	auto addOp = [](uint32_t operand1, uint32_t operand2)->int32_t { return static_cast<int32_t>(operand1) + static_cast<int32_t>(operand2); };
	executeRegisterTypeArithmeticOp("add", addOp, true);
} 

void mips::CPU::addi()
{
	auto addiOp = [](uint32_t operand1, int16_t operand2)->int32_t { return static_cast<int32_t>(operand1) + operand2; };
	executeImmediateTypeArithmeticOp("addi", addiOp, true);
}

void mips::CPU::addiu()
{

	auto addiOp = [](uint32_t operand1, int16_t operand2)->int32_t { return static_cast<int32_t>(operand1) + operand2; };
	executeImmediateTypeArithmeticOp("addiu", addiOp, false);
}

void mips::CPU::addu()
{
	auto addOp = [](uint32_t operand1, uint32_t operand2)->int32_t { return static_cast<int32_t>(operand1) + static_cast<int32_t>(operand2); };
	executeRegisterTypeArithmeticOp("addu", addOp, false);
}

void mips::CPU::and_()
{
	auto andOp = [](uint32_t operand1, uint32_t operand2)->int32_t { return operand1 & operand2; };
	executeRegisterTypeArithmeticOp("and", andOp, true);
}

void mips::CPU::andi()
{
	auto andiOp = [](uint32_t operand1, uint16_t operand2)->int32_t { return operand1 & operand2; };
	executeImmediateTypeArithmeticOp("andi", andiOp, true);
}

void mips::CPU::beq()
{
	auto equals = [](int32_t operand1, int32_t operand2)->bool { return static_cast<int32_t>(operand1) == static_cast<int32_t>(operand2); };
	executeBranchOp("beq", equals, false);
}

void mips::CPU::bgez()
{
	auto greaterEqualToZero = [](uint32_t operand1, uint32_t operand2)->bool { return static_cast<int32_t>(operand1) >= 0; };
	executeBranchOp("bgez", greaterEqualToZero, true);
}

void mips::CPU::bgezal()
{
	m_registerFile[cpu_constants::LINK_REGISTER] = m_pc + (cpu_constants::WORD_SIZE * 2);
	auto greaterEqualToZero = [](uint32_t operand1, uint32_t operand2)->bool { return static_cast<int32_t>(operand1) >= 0; };
	executeBranchOp("bgezal", greaterEqualToZero, true);
}

void mips::CPU::bgtz()
{
	auto greaterThanZero = [](uint32_t operand1, uint32_t operand2)->bool { return static_cast<int32_t>(operand1) > 0;  };
	executeBranchOp("bgtz", greaterThanZero, true);
}

void mips::CPU::blez()
{
	auto lessEqualToZero = [](uint32_t operand1, uint32_t operand2)->bool { return static_cast<int32_t>(operand1) <= 0;  };
	executeBranchOp("blez", lessEqualToZero, true);
}

void mips::CPU::bltz()
{
	auto lessThanZero = [](uint32_t operand1, uint32_t operand2)->bool { return static_cast<int32_t>(operand1) < 0; };
	executeBranchOp("bltz", lessThanZero, true);
}

void mips::CPU::bltzal()
{
	m_registerFile[cpu_constants::LINK_REGISTER] = m_pc + (cpu_constants::WORD_SIZE * 2);
	auto lessThanZero = [](uint32_t operand1, uint32_t operand2)->bool { return static_cast<int32_t>(operand1) < 0; };
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