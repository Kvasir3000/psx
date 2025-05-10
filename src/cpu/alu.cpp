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

}

void mips::CPU::fillSecondaryOpcodeTable()
{
	m_secondaryOpcodeTable[ADD]   = std::bind(&CPU::add,    this);
	m_secondaryOpcodeTable[ADDU]  = std::bind(&CPU::addu,   this);
	m_secondaryOpcodeTable[AND]   = std::bind(&CPU::and_,   this);
	m_secondaryOpcodeTable[BREAK] = std::bind(&CPU::break_, this);
	m_secondaryOpcodeTable[DIV]   = std::bind(&CPU::div,    this);
	m_secondaryOpcodeTable[DIVU]  = std::bind(&CPU::divu,   this);
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

void mips::CPU::add()
{
	auto addOp = [](int32_t operand1, int32_t operand2)->int32_t { return operand1 + operand2; };
	ArithmeticOpFlags opFlags = { true, false, false };
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
	ArithmeticOpFlags opFlags = {false, false, false};
	//executeRegisterTypeArithmeticOp("addu", addOp, false, false);
}

void mips::CPU::and_()
{
	auto andOp = [](uint32_t operand1, uint32_t operand2)->int32_t { return operand1 & operand2; };
	//executeRegisterTypeArithmeticOp("and", andOp, true, false);
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
	auto divide = [this](int32_t operand1, int32_t operand2)->uint32_t
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
	executeRegisterTypeArithmeticOp("div", divide, opFlags);
}

void mips::CPU::divu()
{
	auto divide = [this](uint32_t operand1, uint32_t operand2)->uint32_t
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
	executeRegisterTypeArithmeticOp("divu", divide, opFlags);
}