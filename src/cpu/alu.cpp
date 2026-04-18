#include <assert.h>

#include "../../inc/cpu/cpu.h"
#include "../../inc/constants/debugger_constants.h"



void mips::CPU::fillPrimaryOpcodeTable()
{
	m_primaryOpcodeTable[ADDI]  = std::bind(&CPU::addi,  this);
	m_primaryOpcodeTable[ADDIU] = std::bind(&CPU::addiu, this);
	m_primaryOpcodeTable[ANDI]  = std::bind(&CPU::andi,  this);
	m_primaryOpcodeTable[BEQ]   = std::bind(&CPU::beq,   this);
	m_primaryOpcodeTable[BNE]   = std::bind(&CPU::bne,   this);
	m_primaryOpcodeTable[COP0_] = std::bind(&CPU::cop,   this);
	m_primaryOpcodeTable[COP2]  = std::bind(&CPU::cop,   this);
	m_primaryOpcodeTable[J]     = std::bind(&CPU::jump,  this);
	m_primaryOpcodeTable[JAL]   = std::bind(&CPU::jal,   this);
	m_primaryOpcodeTable[LB]    = std::bind(&CPU::lb ,   this);
	m_primaryOpcodeTable[LBU]   = std::bind(&CPU::lbu,   this);
	m_primaryOpcodeTable[LH]    = std::bind(&CPU::lh,    this);
	m_primaryOpcodeTable[LHU]   = std::bind(&CPU::lhu,   this);
	m_primaryOpcodeTable[LUI]   = std::bind(&CPU::lui,   this);
	m_primaryOpcodeTable[LW]    = std::bind(&CPU::lw,    this);
	m_primaryOpcodeTable[LWC2]  = std::bind(&CPU::lwc2,  this);
	m_primaryOpcodeTable[LWL]   = std::bind(&CPU::lwl ,  this);
	m_primaryOpcodeTable[LWR]   = std::bind(&CPU::lwr,   this);
	m_primaryOpcodeTable[ORI]   = std::bind(&CPU::ori,   this);
	m_primaryOpcodeTable[SB]    = std::bind(&CPU::sb,    this);
	m_primaryOpcodeTable[SH]    = std::bind(&CPU::sh,    this);
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
	m_secondaryOpcodeTable[MFHI]  = std::bind(&CPU::mfhi,   this);
	m_secondaryOpcodeTable[MFLO]  = std::bind(&CPU::mflo,   this);
	m_secondaryOpcodeTable[MTHI]  = std::bind(&CPU::mthi,   this);
	m_secondaryOpcodeTable[MTLO]  = std::bind(&CPU::mtlo,   this);
	m_secondaryOpcodeTable[MULT]  = std::bind(&CPU::mult,   this);
	m_secondaryOpcodeTable[MULTU] = std::bind(&CPU::multu,  this);
	m_secondaryOpcodeTable[NOR]   = std::bind(&CPU::nor,    this);
	m_secondaryOpcodeTable[OR]    = std::bind(&CPU::_or,     this);
	m_secondaryOpcodeTable[SLL]   = std::bind(&CPU::sll,    this);
	m_secondaryOpcodeTable[SLLV]  = std::bind(&CPU::sllv,   this);
	m_secondaryOpcodeTable[SLT]   = std::bind(&CPU::slt,    this);
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
	m_copOpcodeTable[MFC] = std::bind(&CPU::mfc,  this);
	m_copOpcodeTable[MTC] = std::bind(&CPU::mtc,  this);

}

template <typename Type, typename ArithmeticOp>
void mips::CPU::executeRegisterArithmeticOp(const std::string& mnemonic, const ArithmeticOp& arithmeticOp, const ArithmeticOpFlags& opFlags)
{
	uint32_t rd_idx = m_instruction.getRD();
	uint32_t rs_idx = m_instruction.getRS();
	uint32_t rt_idx = m_instruction.getRT();

	Register<Type> rd = Register<Type>(rd_idx, m_registerFile[rd_idx]);
	Register<Type> rs = Register<Type>(rs_idx, m_registerFile[rs_idx]);
	Register<Type> rt = Register<Type>(rt_idx, m_registerFile[rt_idx]);


	Type result = arithmeticOp(rs.value, rt.value);

	
	if (m_context->isDebug())
	{
		if (!opFlags.isMultiplicative)
		{
			rd.set_value(result);
			m_context->getDebugger()->logGenericRegOp(mnemonic, rd, rs, rt);
		}
		else
		{
			m_context->getDebugger()->logRegisterMultiplicativeArithmetic(mnemonic, (Type)m_hi, (Type)m_lo, rs, rt);
		}
	}

	if (opFlags.catchException && checkOverflow(rs.value, rt.value, result))
	{
		raiseException("Integer overflow");
	}

	if (!opFlags.isMultiplicative) // In case of mul/div results are stored from lambda function
	{
		m_registerFile[rd_idx] = static_cast<uint32_t>(result);
	}
}


template<typename Type, typename ArithmeticOp>
void mips::CPU::executeImmediateArithmeticOp(const std::string& mnemonic, const ArithmeticOp& arithmeticOp, bool catchException)
{
	uint32_t  rs_idx = m_instruction.getRS();
	uint32_t  rt_idx = m_instruction.getRT();

	Register<Type>  rs = Register<Type>(rs_idx, m_registerFile[rs_idx]);
	Register<Type>  rt = Register<Type>(rt_idx, m_registerFile[rt_idx]);
	Immediate<Type> immediate = Immediate<Type>(m_instruction.getImmediate());

	Type result = arithmeticOp(rs.value, immediate.value);
	if (m_context->isDebug())
	{
		rt.set_value(result);
		m_context->getDebugger()->logGenericRegOp(mnemonic, rt, rs, immediate);
	}

	if (catchException && checkOverflow(m_registerFile[rs_idx], immediate.value, result))
	{
		raiseException("Integer overflow");
	}

	m_registerFile[rt_idx] = static_cast<uint32_t>(result);
}

void mips::CPU::executeBranchOp(const std::string& mnemonic, const std::function<bool(uint32_t, uint32_t)>& branchCondition, bool compareToZero)
{
	uint32_t rs = m_instruction.getRS();
	uint32_t rt = m_instruction.getRT();
	int16_t  offset = m_instruction.getImmediate();

	if (branchCondition(m_registerFile[rs], m_registerFile[rt]))
	{
		m_delaySlot.status = cpu_constants::DelaySlotState::Pending;
		m_delaySlot.targetAddress = m_pc + cpu_constants::WORD_SIZE_BYTES + (offset << 2);
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

void mips::CPU::executeJumpOp(const std::string& mnemonic)
{
	uint32_t target = m_instruction.getJumpTarget();
	m_delaySlot.status = cpu_constants::DelaySlotState::Pending;
	m_delaySlot.targetAddress = ((m_pc + cpu_constants::WORD_SIZE_BYTES) & cpu_constants::PC_HIGH_BITS_MASK) | (target << 2);

	if (m_context->isDebug())
	{
		m_context->getDebugger()->logJump("j", m_delaySlot.targetAddress);
	}
}

void mips::CPU::excecuteJumpRegisterOp(const std::string& mnemonic)
{
	uint32_t rd = m_instruction.getRD();
	uint32_t rs = m_instruction.getRS();
	
	if (rd) // if rd is 0, means that this is not link instruction
	{
		assert(m_instruction.getSecondaryOpcode() == JALR);
		m_registerFile[rd] = m_pc + (2 * cpu_constants::WORD_SIZE_BYTES);
	}
	m_delaySlot.status = cpu_constants::DelaySlotState::Pending; 
	m_delaySlot.targetAddress = m_registerFile[rs];

	if (m_context->isDebug())
	{
		m_context->getDebugger()->logJumpRegister(mnemonic, rs, m_registerFile[rs]);
	}
}

void mips::CPU::executeLoadOp(const std::string& mnemonic, const std::function<uint32_t(uint32_t)>& loadOp, const LoadOpFlags& opFlags)
{
	uint32_t rt = m_instruction.getRT();
	uint32_t base = m_instruction.getBase();
	int32_t  offset = m_instruction.getOffset();

	uint32_t address = m_registerFile[base] + offset;

	if (opFlags.size != cpu_constants::LoadSize::Byte && (address & opFlags.alignMask))
	{
		m_context->getDebugger()->logWarning("Accessing unaligned memory:");
	}

	if (opFlags.isCop2)
	{
		/*
		* Might want to consider adding a seperate logging function where the name of each 
		* COP2 data register will be displayed
		*/
		uint32_t word = m_context->getBus()->readWord(address);
		m_gte.writeDataRegister(rt, word);
	} 
	else
	{
		DelayLoad load;
		load.status = cpu_constants::DelaySlotState::Pending;
		load.data = loadOp(address);
		load.registerIdx = rt; 
		load.loadSize = opFlags.size;
		load.sign = opFlags.isSigned;

		m_delayLoads.emplace(load);
	}

	if (m_context->isDebug())
	{
		m_context->getDebugger()->logMemoryOperation(mnemonic, rt, offset, base, m_registerFile[base]);
	}
}

void mips::CPU::executeLoadWordLROp(const std::string& mnemonic, const std::function<uint32_t(uint32_t, uint32_t, uint32_t)>& adjustWord)
{
	// Have no idea if this is working correctly, it is passing my test, but due to big/little endian difference my understanding of this may be wrong, check other emulators 
	uint32_t rt = m_instruction.getRT();
	int16_t  offset = m_instruction.getOffset();
	uint32_t base = m_instruction.getBase();

	uint32_t address = m_registerFile[base] + offset;
	uint32_t alignedAddress = address & ~cpu_constants::WORD_ALIGNED_MASK;
	uint32_t word = m_context->getBus()->readWord(alignedAddress);

	uint32_t result = adjustWord(rt, address, word);

	if (m_context->isDebug())
	{
		m_context->getDebugger()->logMemoryOperation("lwr", rt, offset, base, m_registerFile[base]);
		m_context->getDebugger()->logLoadShift(address, alignedAddress, word, rt, m_registerFile[rt], result);
	}

	m_registerFile[rt] = result;
}

template<typename MovOp>
void mips::CPU::executeMovHiLo(const std::string& mnemonic, const MovOp& movOp)
{
	uint32_t register_idx = movOp();

	if (m_context->isDebug())
	{
		Register<uint32_t> reg = Register<uint32_t>(register_idx, m_registerFile[register_idx]);
		m_context->getDebugger()->logGenericRegOp(mnemonic, reg);
	}
}

void mips::CPU::executeStoreOp(const std::string& mnemonic, const std::function<void(uint32_t, uint32_t)>& storeOp)
{
	uint32_t rt = m_instruction.getRT();
	uint32_t offset = m_instruction.getOffset(); 
	uint32_t base = m_instruction.getBase(); 

	uint32_t address = offset + m_registerFile[base];
	storeOp(address, m_registerFile[rt]);
	
	if (m_context->isDebug())
	{
		m_context->getDebugger()->logMemoryOperation(mnemonic, rt, offset, base, m_registerFile[base]);
	}

	raiseException("Store exceptions are not implemented yet"); 
}

void mips::CPU::executeShiftOp(const std::string& mnemonic, const std::function<uint32_t(uint32_t, uint32_t&)>& shiftOp, bool isLogical)
{
	uint32_t rt = m_instruction.getRT();
	uint32_t rd = m_instruction.getRD();
	uint32_t shift = 0; 

	uint32_t result = shiftOp(m_registerFile[rt], shift);

	if (m_context->isDebug())
	{
		if (isLogical)
		{
			m_context->getDebugger()->logShiftLogical(mnemonic, rd, rt, shift, result, m_registerFile[rt]);

		}
		else
		{
			m_context->getDebugger()->logShiftVariable(mnemonic, rd, rt, m_instruction.getRS(), result, m_registerFile[rt], shift);
		}
	}

	m_registerFile[rd] = result;
}


template <typename Type, typename SetOp>
void mips::CPU::executeRegisterSetOnOp(const std::string& mnemonic, const SetOp& setOp)
{
	uint32_t rt_idx = m_instruction.getRT();
	uint32_t rs_idx = m_instruction.getRS();
	uint32_t rd_idx = m_instruction.getRD();

	Register<Type> rt = Register<Type>(rt_idx, m_registerFile[rt_idx]);
	Register<Type> rs = Register<Type>(rs_idx, m_registerFile[rs_idx]);
	Register<Type> rd = Register<Type>(rd_idx, m_registerFile[rd_idx]);

	m_registerFile[rd_idx] = setOp(rt.value, rs.value);

	if (m_context->isDebug())
	{
		rd.set_value(m_registerFile[rd_idx]);
		m_context->getDebugger()->logGenericRegOp(mnemonic, rd, rs, rt);
	}
}

template <typename Type, typename SetOp>
void mips::CPU::executeImmediateSetOnOp(const std::string& mnemonic, const SetOp& setOp)
{
	uint32_t rt_idx = m_instruction.getRT();
	uint32_t rs_idx = m_instruction.getRS();
	uint32_t rd_idx = m_instruction.getRD();

	Register<Type> rt = Register<Type>(rt_idx, m_registerFile[rt_idx]);
	Register<Type> rs = Register<Type>(rs_idx, m_registerFile[rs_idx]);
	Register<Type> rd = Register<Type>(rd_idx, m_registerFile[rd_idx]);

	m_registerFile[rd_idx] = setOp(rt.value, rs.value);

	if (m_context->isDebug())
	{
		rd.set_value(m_registerFile[rd_idx]);
		m_context->getDebugger()->logGenericRegOp(mnemonic, rd, rs, rt);
	}
}

void mips::CPU::add()
{
	auto addOp = [](int32_t operand1, int32_t operand2)->int32_t { return operand1 + operand2; };
	ArithmeticOpFlags opFlags = { true, false };
	executeRegisterArithmeticOp<int32_t>("add", addOp, opFlags);
} 

void mips::CPU::addi()
{
	auto addiOp = [](int32_t operand1, int16_t operand2)->int32_t { return operand1 + operand2; };
	executeImmediateArithmeticOp<int32_t>("addi", addiOp, true);
}

void mips::CPU::addiu()
{
	auto addiOp = [](uint32_t operand1, int16_t operand2)->int32_t { return operand1 + operand2; };
	executeImmediateArithmeticOp<uint32_t>("addiu", addiOp, false);
}

void mips::CPU::addu()
{
	auto addOp = [](int32_t operand1, int32_t operand2)->int32_t { return operand1 + operand2; };
	ArithmeticOpFlags opFlags = { false, false };
	executeRegisterArithmeticOp<uint32_t>("addu", addOp, opFlags);
}

void mips::CPU::and_()
{
	auto andOp = [](uint32_t operand1, uint32_t operand2)->int32_t { return operand1 & operand2; };
	ArithmeticOpFlags opFlags = { false, false };
	executeRegisterArithmeticOp<uint32_t>("and", andOp, opFlags);
}

void mips::CPU::andi()
{
	auto andiOp = [](uint32_t operand1, uint16_t operand2)->int32_t { return operand1 & operand2; };
	executeImmediateArithmeticOp<uint32_t>("andi", andiOp, true);
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
	m_registerFile[cpu_constants::LINK_REGISTER] = m_pc + (cpu_constants::WORD_SIZE_BYTES * 2);
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
	m_registerFile[cpu_constants::LINK_REGISTER] = m_pc + (cpu_constants::WORD_SIZE_BYTES * 2);
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
	uint32_t rd_idx = m_instruction.getRD();
	uint32_t rt_idx = m_instruction.getRT();
	m_registerFile[rt_idx] = m_gte.readControlRegister(rd_idx);

	if (m_context->isDebug())
	{
		Register<uint32_t> rt = Register<uint32_t>(rt_idx, m_registerFile[rt_idx]);
		Register<uint32_t> rd = Register<uint32_t>(rd_idx, m_registerFile[rt_idx]); 

		//m_context->getDebugger()->logGenericRegOp("cfc2", rt, rd);
		m_context->getDebugger()->logMove("cfc2", rt_idx, rd_idx, m_registerFile[rt_idx]);
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
	ArithmeticOpFlags opFlags = { false, true };
	executeRegisterArithmeticOp<int32_t>("div", divideOp, opFlags);
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
	ArithmeticOpFlags opFlags = { false, true };
	executeRegisterArithmeticOp<uint32_t>("divu", divideOp, opFlags);
}

void mips::CPU::jump()
{
	executeJumpOp("j");
}

void mips::CPU::jal()
{
	m_registerFile[cpu_constants::LINK_REGISTER] = m_pc + (cpu_constants::WORD_SIZE_BYTES * 2);
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
	LoadOpFlags opFlags = {cpu_constants::LoadSize::Byte, true };
	executeLoadOp("lb", loadByteOp, opFlags);
}

void mips::CPU::lbu()
{
	auto loadByteOp = [this](uint32_t memoryAddress)->uint32_t { return m_context->getBus()->readByte(memoryAddress); };
	LoadOpFlags opFlags = { cpu_constants::LoadSize::Byte, false };
	executeLoadOp("lbu", loadByteOp, opFlags);
}

void mips::CPU::lh()
{
	auto loadHalfwordOp = [this](uint32_t memoryAddress)->int32_t { return m_context->getBus()->readHalfword(memoryAddress); };
	LoadOpFlags opFlags = { cpu_constants::LoadSize::Halfword, true, cpu_constants::HALFWORD_ALIGNED_MASK};
	executeLoadOp("lh", loadHalfwordOp, opFlags);
};

void mips::CPU::lhu()
{
	auto loadHalfwordOp = [this](uint32_t memoryAddress)->uint32_t { return m_context->getBus()->readHalfword(memoryAddress); };
	LoadOpFlags opFlags = { cpu_constants::LoadSize::Halfword, false, cpu_constants::HALFWORD_ALIGNED_MASK };
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
	LoadOpFlags opFlags = { cpu_constants::LoadSize::Word, false, cpu_constants::WORD_ALIGNED_MASK};
	executeLoadOp("lw", loadWord, opFlags);
}

/*
 I am not sure if delay slot is needed here, based on the no$ looks like yes:
 "Writing to cop2 registers has a delay of 2..3 clock cycles"
 But for now i am loading it immediately
*/
void mips::CPU::lwc2()
{
	auto loadWord = [this](uint32_t memoryAddress)->uint32_t { return m_context->getBus()->readWord(memoryAddress); };
	LoadOpFlags opFlags = { cpu_constants::LoadSize::Word, false, cpu_constants::WORD_ALIGNED_MASK, true };
	executeLoadOp("lwc2", loadWord, opFlags);
	
	raiseException("Coprocessor unusable bit is not implemented yet");
	raiseException("Coprocessor unusable");
}

void mips::CPU::lwl()
{
	// Have no idea if this is working correctly, it is passing my test, but due to big/little endian difference my understanding of this may be wrong, check other emulators 
	auto adjustLWLOp = [this](uint32_t rt, uint32_t address, uint32_t word)->uint32_t
	{
		uint32_t wordOffset = (cpu_constants::WORD_ALIGNED_MASK - (address & cpu_constants::WORD_ALIGNED_MASK)) * 8;
		uint32_t adjustedWord = word << wordOffset;
		uint32_t rightSideMask = (cpu_constants::WORD_MASK >> (cpu_constants::WORD_SIZE_BITS - wordOffset));
		uint32_t result = adjustedWord | (m_registerFile[rt] & rightSideMask);
		return result;
	};

	executeLoadWordLROp("lwl", adjustLWLOp);
}

void mips::CPU::lwr()
{
	// Have no idea if this is working correctly, it is passing my test, but due to big/little endian difference my understanding of this may be wrong, check other emulators 
	auto adjustLWROp = [this](uint32_t rt, uint32_t address, uint32_t word)->uint32_t
	{
		uint32_t wordOffset = (address & cpu_constants::WORD_ALIGNED_MASK) * 8;
		uint32_t adjustedWord = word >> wordOffset;
		uint32_t leftSideMask = (cpu_constants::WORD_MASK << (cpu_constants::WORD_SIZE_BITS - wordOffset));
		uint32_t result = adjustedWord | (m_registerFile[rt] & leftSideMask);
		return result;
	};

	executeLoadWordLROp("lwr", adjustLWROp);
}
 
void mips::CPU::mfc()
{
	uint32_t rd_idx = m_instruction.getRD();
	uint32_t rt_idx = m_instruction.getRT();
	m_registerFile[rt_idx] = (m_instruction.getCOPIdx() == 0) ? m_cop0.readDataRegister(rd_idx) : m_gte.readDataRegister(rd_idx);

	if (m_context->isDebug())
	{
		std::string mnemonic = (m_instruction.getCOPIdx() == 0)? "mfc0" : "mfc2";
		Register<uint32_t> rd = Register<uint32_t>(rd_idx, m_registerFile[rt_idx]); // to keep logs simpler read from rt intentionally
		Register<uint32_t> rt = Register<uint32_t>(rt_idx, m_registerFile[rt_idx]);
		m_context->getDebugger()->logGenericRegOp(mnemonic, rt, rd);
	}
	raiseException("Coprocessor unusable");
}

void mips::CPU::mfhi()
{
	auto movFromHiOp = [this]()->uint32_t {	uint32_t rd_idx = m_instruction.getRD(); m_registerFile[rd_idx] = m_hi; return rd_idx; };
	executeMovHiLo("mfhi", movFromHiOp);
}

void mips::CPU::mflo()
{
	auto movFromLoOp = [this]()->uint32_t {	uint32_t rd_idx = m_instruction.getRD(); m_registerFile[rd_idx] = m_lo; return rd_idx; };
	executeMovHiLo("mflo", movFromLoOp);
}

void mips::CPU::mtc()
{
	uint32_t rd_idx = m_instruction.getRD();
	uint32_t rt_idx = m_instruction.getRT();

	if (m_instruction.getCOPIdx() == 0)
	{
		m_cop0.writeDataRegister(rd_idx, m_registerFile[rt_idx]);
	}
	else
	{
		m_gte.writeDataRegister(rd_idx, m_registerFile[rt_idx]);
	}
	
	if (m_context->isDebug())
	{
		std::string mnemonic = (m_instruction.getCOPIdx() == 0) ? "mtc0" : "mtc2";
		Register<uint32_t> rd = Register<uint32_t>(rd_idx, m_registerFile[rt_idx]); // to keep logs simpler read from rd intentionally
		Register<uint32_t> rt = Register<uint32_t>(rt_idx, m_registerFile[rt_idx]); 
		m_context->getDebugger()->logGenericRegOp(mnemonic, rd, rt);
	}
	raiseException("Coprocessor unusable");
}

void mips::CPU::mthi()
{
	auto movToHiOp = [this]()->uint32_t { uint32_t rs_idx = m_instruction.getRS(); m_hi = m_registerFile[rs_idx]; return rs_idx; };
	executeMovHiLo("mthi", movToHiOp);
}

void mips::CPU::mtlo()
{
	auto movToLoOp = [this]()->uint32_t { uint32_t rs_idx = m_instruction.getRS(); m_lo = m_registerFile[rs_idx]; return rs_idx; };
	executeMovHiLo("mtlo", movToLoOp);
}

void mips::CPU::mult()
{
	auto multiplyOp = [this](int32_t operand1, int32_t operand2)->int32_t
    {
	     int64_t result = static_cast<int64_t>(operand1) * static_cast<int64_t>(operand2);
		 m_lo = result & cpu_constants::WORD_MASK;
		 m_hi = (result >> cpu_constants::WORD_SIZE_BITS) & cpu_constants::WORD_MASK;

		 return 0;
	};
	ArithmeticOpFlags opFlags = { false, true };
	executeRegisterArithmeticOp<int32_t>("mult", multiplyOp, opFlags);
}

void mips::CPU::multu()
{
	auto multiplyOp = [this](uint32_t operand1, uint32_t operand2)
	{
		uint64_t result = static_cast<uint64_t>(operand1) * static_cast<uint64_t>(operand2);
		m_lo = result & cpu_constants::WORD_MASK;
		m_hi = (result >> cpu_constants::WORD_SIZE_BITS) & cpu_constants::WORD_MASK;

		return 0;
	};
	ArithmeticOpFlags opFlags = { false, true };
	executeRegisterArithmeticOp<uint32_t>("multu", multiplyOp, opFlags);
}

void mips::CPU::nor()
{
	auto norOp = [](uint32_t operand1, uint32_t operand2)->uint32_t { return ~(operand1 | operand2); };
	ArithmeticOpFlags opFlags = { false, false };
	executeRegisterArithmeticOp<uint32_t>("nor", norOp, opFlags);
}

void mips::CPU::_or()
{
	auto orOp = [](uint32_t operand1, uint32_t operand2)->uint32_t { return operand1 | operand2; };
	ArithmeticOpFlags opFlags = { false, false };
	executeRegisterArithmeticOp<uint32_t>("or", orOp, opFlags);
}

void mips::CPU::ori()
{
	auto oriOp = [](uint32_t operand1, uint16_t operand2)->uint32_t { return operand1 | operand2;  };
	executeImmediateArithmeticOp<uint32_t>("ori", oriOp, false);
}

void mips::CPU::sb()
{
	auto sbOp = [this](uint32_t address, uint32_t data)->void { m_context->getBus()->storeByte(address, data); };
	executeStoreOp("sb", sbOp);
}

void mips::CPU::sh()
{
	auto shOp = [this](uint32_t address, uint32_t data)->void { m_context->getBus()->storeHalfword(address, data); };
	executeStoreOp("sh", shOp);
}

void mips::CPU::sll()
{
	auto shiftLeftLogicalOp = [this](uint32_t value, uint32_t& shift)->uint32_t { shift = m_instruction.getSA(); return value << shift; };
	executeShiftOp("sll", shiftLeftLogicalOp, true);
}

void mips::CPU::sllv()
{
	auto shiftLeftVariableOp = [this](uint32_t value, uint32_t& shift)->uint32_t { shift = m_registerFile[m_instruction.getRS()]; return value << shift; };
	executeShiftOp("sllv", shiftLeftVariableOp, false);
}

void mips::CPU::slt()
{
	auto setOnLessThanOp = [](int32_t rt, int32_t rs)->bool { return rs < rt;  };
	executeRegisterSetOnOp<int32_t>("slt", setOnLessThanOp);
}

void mips::CPU::slti()
{
	auto setOnLessThanImmOp = [](int32_t rs, int32_t immediate)->bool { return rs < immediate; };
	executeRegisterSetOnOp<int32_t>("slti", setOnLessThanImmOp);
}

