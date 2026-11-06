#include <assert.h>

#include "../../inc/cpu/cpu.h"
#include "../../inc/constants/debugger_constants.h"

template <typename Type, typename ArithmeticOp>
void mips::CPU::executeRegisterArithmeticOp(const std::string& mnemonic, const ArithmeticOp& arithmeticOp, bool catchException)
{
	uint32_t rdIdx = m_instruction.getRD();
	uint32_t rsIdx = m_instruction.getRS();
	uint32_t rtIdx = m_instruction.getRT();

	Register<Type> rs = Register<Type>(rsIdx, m_registerFile[rsIdx]);
	Register<Type> rt = Register<Type>(rtIdx, m_registerFile[rtIdx]);

	Type result = arithmeticOp(rs.value, rt.value);
	m_registerFile[rdIdx] = static_cast<uint32_t>(result); // ?

	
	if (m_context->isDebug())
	{
		Register<Type> rd = Register<Type>(rdIdx, m_registerFile[rdIdx]);
		m_debugger->logGenericRegOp(mnemonic, rd, rs, rt);		
	}

	if (catchException && checkOverflow(rs.value, rt.value, result))
	{
		raiseException("Integer overflow");
	}
}

template<typename Type, typename HiLoArithmeticOp>
void mips::CPU::executeHiLoArithmeticOp(const std::string& mnemonic, const HiLoArithmeticOp& arithmeticOp)
{
	uint32_t rsIdx = m_instruction.getRS();
	uint32_t rtIdx = m_instruction.getRT();

	Register<Type> rs = Register<Type>(rsIdx, m_registerFile[rsIdx]);
	Register<Type> rt = Register<Type>(rtIdx, m_registerFile[rtIdx]);

	arithmeticOp(rs.value, rt.value);

	if (m_context->isDebug())
	{
		m_debugger->logHiLoArithmeticOp(mnemonic, (Type)m_hi, (Type)m_lo, rs, rt);
	}
}

template<typename Type, typename ArithmeticOp>
void mips::CPU::executeImmediateArithmeticOp(const std::string& mnemonic, const ArithmeticOp& arithmeticOp, bool catchException)
{
	uint32_t  rsIdx = m_instruction.getRS();
	uint32_t  rtIdx = m_instruction.getRT();

	Register<Type>  rs = Register<Type>(rsIdx, m_registerFile[rsIdx]);
	Register<Type>  rt = Register<Type>(rtIdx, m_registerFile[rtIdx]);
	Immediate<Type> immediate = Immediate<Type>(m_instruction.getImmediate());

	Type result = arithmeticOp(rs.value, immediate.value);
	if (m_context->isDebug())
	{
		rt.set_value(result);
		m_debugger->logGenericRegOp(mnemonic, rt, rs, immediate);
	}

	if (catchException && checkOverflow(m_registerFile[rsIdx], immediate.value, result))
	{
		raiseException("Integer overflow");
	}

	m_registerFile[rtIdx] = static_cast<uint32_t>(result);
}


template<typename BranchZeroOp>
void mips::CPU::executeBranchZeroOp(const std::string& mnemonic, const BranchZeroOp& branchCondition)
{
	uint32_t			rsIdx = m_instruction.getRS();
	Register<int32_t>   rs = Register<int32_t>(rsIdx, m_registerFile[rsIdx]);
	Immediate<int16_t>  offset = m_instruction.getOffset();

	if (branchCondition(rs.value))
	{
		m_delaySlot.status = cpu_constants::DelaySlotState::Pending;
		m_delaySlot.targetAddr = m_pc + cpu_constants::WORD_SIZE_BYTES + (offset.value << 2);
	}

	if (m_context->isDebug())
	{
		bool ignoreBranch = m_delaySlot.status != cpu_constants::DelaySlotState::Pending;
		m_debugger->logBranchOp(mnemonic, ignoreBranch, m_delaySlot.targetAddr, rs, offset);
	}
};

template<typename BranchOp>
void mips::CPU::executeBranchOp(const std::string& mnemonic, const BranchOp& branchCondition)
{
	uint32_t			rsIdx = m_instruction.getRS();
	uint32_t			rtIdx = m_instruction.getRT();
	Register<int32_t>   rs = Register<int32_t>(rsIdx, m_registerFile[rsIdx]);
	Register<int32_t>   rt = Register<int32_t>(rtIdx, m_registerFile[rtIdx]);
	Immediate<int16_t>  offset = m_instruction.getOffset();

	if (branchCondition(rs.value, rt.value))
	{
		m_delaySlot.status = cpu_constants::DelaySlotState::Pending;
		m_delaySlot.targetAddr = m_pc + cpu_constants::WORD_SIZE_BYTES + (offset.value << 2);
	}
	
	if (m_context->isDebug())
	{
		bool ignoreBranch = m_delaySlot.status != cpu_constants::DelaySlotState::Pending;
		m_debugger->logBranchOp(mnemonic, ignoreBranch, m_delaySlot.targetAddr, rs, rt, offset);
	}
}

void mips::CPU::executeJumpOp(const std::string& mnemonic)
{
	uint32_t target = m_instruction.getJumpTarget();
	m_delaySlot.status = cpu_constants::DelaySlotState::Pending;
	m_delaySlot.targetAddr = ((m_pc + cpu_constants::WORD_SIZE_BYTES) & cpu_constants::PC_HIGH_BITS_MASK) | (target << 2);
	if (m_context->isDebug())
	{
		m_debugger->logBranchOp(mnemonic, false, m_delaySlot.targetAddr, Immediate<uint32_t>(target));
	}
}

void mips::CPU::excecuteJumpRegisterOp(const std::string& mnemonic)
{
	uint32_t rdIdx = m_instruction.getRD();
	uint32_t rsIdx = m_instruction.getRS();
	Register<int32_t> rs = Register<int32_t>(rsIdx, m_registerFile[rsIdx]);
	
	if (rdIdx) // if rd is 0, means that this is not link instruction
	{
		assert(m_instruction.getSecondaryOpcode() == JALR);
		m_registerFile[rdIdx] = m_pc + (2 * cpu_constants::WORD_SIZE_BYTES);
	}
	m_delaySlot.status = cpu_constants::DelaySlotState::Pending; 
	m_delaySlot.targetAddr = rs.value;

	if (m_context->isDebug())
	{
		m_debugger->logGenericRegOp(mnemonic, rs);
	}
}

template <typename Type, typename LoadOp>
void mips::CPU::executeLoadOp(const std::string& mnemonic, const LoadOp& loadOp)
{
	uint32_t            rtIdx = m_instruction.getRT();
	uint32_t            baseIdx = m_instruction.getBase();

	Register<uint32_t>	rt = Register<uint32_t>(rtIdx, m_registerFile[rtIdx]);
	Register<uint32_t>  base = Register<uint32_t>(baseIdx, m_registerFile[baseIdx]);
	Immediate<int32_t>  offset = Immediate<int32_t>(m_instruction.getOffset());

	uint32_t addr = static_cast<uint32_t>(base.value + offset.value);

	DelayLoad load;
	load.status = cpu_constants::DelaySlotState::Pending;
	load.data = loadOp(addr);
	load.registerIdx = rtIdx; 
	load.size = static_cast<cpu_constants::LoadSize>(sizeof(Type));
	load.sign = std::is_signed_v<Type>;

	m_delayLoads.emplace(load);

	if (m_context->isDebug())
	{
		m_debugger->logMemoryOp(mnemonic, rt, offset, base);
	}

	if (isAligned(addr, load.size))
	{
		m_debugger->logWarning("Accessed unaligned memory ^");;
	}
}

template<typename AdjustWordOp>
void mips::CPU::executeLoadWordLROp(const std::string& mnemonic, const AdjustWordOp& adjustWord)
{
	uint32_t            rtIdx = m_instruction.getRT();
	Immediate<int32_t>  offset = Immediate<int32_t>(m_instruction.getOffset());
	uint32_t            baseIdx = m_instruction.getBase();

	uint32_t effectiveAddr = m_registerFile[baseIdx] + offset.value;
	uint32_t alignedAddr = effectiveAddr & ~cpu_constants::WORD_ALIGNED_MASK;
	uint32_t word = m_bus->readWord(alignedAddr);
	uint32_t sourceRt = m_registerFile[rtIdx];

	if (!m_delayLoads.empty() && m_delayLoads.front().registerIdx == rtIdx)
	{
		sourceRt = m_delayLoads.front().data; // merge it with a previous rt call
	}
	uint32_t result = adjustWord(sourceRt, effectiveAddr, word);

	if (m_context->isDebug())
	{
		Register<uint32_t> base = Register<uint32_t>(baseIdx, m_registerFile[baseIdx]);
		Register<uint32_t> rt   = Register<uint32_t>(rtIdx, m_registerFile[rtIdx]);
		m_debugger->logMemoryOp(mnemonic, rt, offset, base);
	}

	DelayLoad load;
	load.status = cpu_constants::DelaySlotState::Pending;
	load.data = result;
	load.registerIdx = rtIdx;
	load.size = cpu_constants::LoadSize::Word;
	load.sign = true;
	if (!m_delayLoads.empty())
	{
		m_delayLoads.front() = load;
	}
	else
	{
		m_delayLoads.emplace(load);
	}
}

template<typename CopOp>
void mips::CPU::executeCopOp(const std::string& mnemonic, const CopOp& copOp)
{
	uint32_t rdIdx = m_instruction.getRD();
	uint32_t rtIdx = m_instruction.getRT();

	copOp(rtIdx, rdIdx);

	if (m_context->isDebug())
	{
		uint32_t		   copIdx = m_instruction.getCOPIdx();
		Register<uint32_t> rt = Register<uint32_t>(rtIdx, m_registerFile[rtIdx]);
		COPRegister        rd = COPRegister(copIdx, rdIdx, m_copMap[copIdx]->readDataRegister(rdIdx));
		m_debugger->logGenericRegOp(mnemonic, rt, rd);
	}
	raiseException("Coprocessor unusable");
}

template<typename CopLoadStoreOp>
void mips::CPU::executeCopLoadStoreOp(const std::string& mnemonic, const CopLoadStoreOp& copOp)
{
	uint32_t            rtIdx = m_instruction.getRT();
	uint32_t            baseIdx = m_instruction.getBase();

	Register<uint32_t>  base = Register<uint32_t>(baseIdx, m_registerFile[baseIdx]);
	Immediate<int32_t>  offset = Immediate<int32_t>(m_instruction.getOffset());

	uint32_t addr = base.value + offset.value;

	copOp(addr, rtIdx);

	if (m_context->isDebug())
	{
		COPRegister	rt = COPRegister(2, rtIdx, m_gte.readDataRegister(rtIdx));
		m_debugger->logMemoryOp(mnemonic, rt, offset, base);
	}


	if (isAligned(addr, cpu_constants::LoadSize::Word))
	{
		m_debugger->logWarning("Accessed unaligned memory ^");
	}

	raiseException("Coprocessor unusable bit is not implemented yet");
	raiseException("Coprocessor unusable");
}

template<typename MovOp>
void mips::CPU::executeMovHiLo(const std::string& mnemonic, const MovOp& movOp)
{
	uint32_t regIdx = movOp();

	if (m_context->isDebug())
	{
		Register<uint32_t> reg = Register<uint32_t>(regIdx, m_registerFile[regIdx]);
		m_debugger->logGenericRegOp(mnemonic, reg);
	}
}

template<typename StoreOp>
void mips::CPU::executeStoreOp(const std::string& mnemonic, const StoreOp& storeOp)
{
	uint32_t            rtIdx = m_instruction.getRT();
	uint32_t            baseIdx = m_instruction.getBase();
	
	Register<uint32_t>  rt = Register<uint32_t>(rtIdx, m_registerFile[rtIdx]);
	Register<uint32_t>  base = Register<uint32_t>(baseIdx, m_registerFile[baseIdx]);
	Immediate<int32_t>  offset = Immediate<int32_t>(m_instruction.getOffset());

	uint32_t addr = offset.value + m_registerFile[baseIdx];
	storeOp(addr, m_registerFile[rtIdx]);
	
	if (m_context->isDebug())
	{
		m_debugger->logMemoryOp(mnemonic, rt, offset, base);
	}

	raiseException("Store exceptions are not implemented yet"); 
}

template<typename SourceValue, typename ShiftValue>
void mips::CPU::executeShiftOp(const std::string& mnemonic, const SourceValue& source, const ShiftValue& shift, bool leftShift)
{
	uint32_t rdIdx = m_instruction.getRD();

	m_registerFile[rdIdx] = leftShift? source.value << shift.value : source.value >> shift.value;

	if (m_context->isDebug())
	{
		Register<uint32_t> rd = Register<uint32_t>(rdIdx, m_registerFile[rdIdx]);
		m_debugger->logGenericRegOp(mnemonic, rd, source, shift);
	}
}


template <typename Type, typename SetOp>
void mips::CPU::executeRegisterSetOnOp(const std::string& mnemonic, const SetOp& setOp)
{
	uint32_t rtIdx = m_instruction.getRT();
	uint32_t rsIdx = m_instruction.getRS();
	uint32_t rdIdx = m_instruction.getRD();

	Register<Type> rt = Register<Type>(rtIdx, m_registerFile[rtIdx]);
	Register<Type> rs = Register<Type>(rsIdx, m_registerFile[rsIdx]);

	m_registerFile[rdIdx] = setOp(rt.value, rs.value);

	if (m_context->isDebug())
	{
		Register<Type> rd = Register<Type>(rdIdx, m_registerFile[rdIdx]);
		m_debugger->logGenericRegOp(mnemonic, rd, rs, rt);
	}
}

template <typename Type, typename SetOp>
void mips::CPU::executeImmediateSetOnOp(const std::string& mnemonic, const SetOp& setOp)
{
	uint32_t rtIdx = m_instruction.getRT();
	uint32_t rsIdx = m_instruction.getRS();

	Immediate<Type> immediate = Immediate<Type>(m_instruction.getImmediate());
	Register<Type>  rs        = Register<Type>(rsIdx, m_registerFile[rsIdx]);

	m_registerFile[rtIdx] = setOp(rs.value, immediate.value);

	if (m_context->isDebug())
	{
		Register<Type> rt = Register<Type>(rtIdx, m_registerFile[rtIdx]);
		m_debugger->logGenericRegOp(mnemonic, rt, rs, immediate);
	}
}

template <typename AdjustWordOp>
void mips::CPU::executeStoreWordLROp(const std::string& mnemonic, const AdjustWordOp& adjustWord)
{
	uint32_t            rtIdx = m_instruction.getRT();
	uint32_t            baseIdx = m_instruction.getBase();

	Register<uint32_t>	rt = Register<uint32_t>(rtIdx, m_registerFile[rtIdx]);
	Register<uint32_t>  base = Register<uint32_t>(baseIdx, m_registerFile[baseIdx]);
	Immediate<int32_t>  offset = Immediate<int32_t>(m_instruction.getOffset());

	uint32_t effectiveAddr = base.value + offset.value;
	uint32_t alignedAddr = effectiveAddr & ~cpu_constants::WORD_ALIGNED_MASK;
	uint32_t memoryWord = m_bus->readWord(alignedAddr);

	uint32_t adjustedWord = adjustWord(memoryWord, rt.value, effectiveAddr & cpu_constants::WORD_ALIGNED_MASK);
	m_bus->storeWord(alignedAddr, adjustedWord);

	if (m_context->isDebug())
	{
		m_debugger->logMemoryOp(mnemonic, rt, offset, base);
	}
}


void mips::CPU::add()
{
	auto addOp = [](int32_t operand1, int32_t operand2)->int32_t { return operand1 + operand2; };
	executeRegisterArithmeticOp<int32_t>("add", addOp, true);
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
	executeRegisterArithmeticOp<uint32_t>("addu", addOp, false);
}

void mips::CPU::and_()
{
	auto andOp = [](uint32_t operand1, uint32_t operand2)->int32_t { return operand1 & operand2; };
	executeRegisterArithmeticOp<uint32_t>("and", andOp, false);
}

void mips::CPU::andi()
{
	auto andiOp = [](uint32_t operand1, uint16_t operand2)->int32_t { return operand1 & operand2; };
	executeImmediateArithmeticOp<uint32_t>("andi", andiOp, true);
}

void mips::CPU::beq()
{
	auto equals = [](int32_t operand1, int32_t operand2)->bool { return operand1 == operand2; };
	executeBranchOp("beq", equals);
}

void mips::CPU::bgez()
{
	auto greaterEqualToZero = [](int32_t operand1)->bool { return operand1 >= 0; };
	executeBranchZeroOp("bgez", greaterEqualToZero);
}

void mips::CPU::bgezal()
{
	m_registerFile[cpu_constants::LINK_REGISTER] = m_pc + (cpu_constants::WORD_SIZE_BYTES * 2);
	auto greaterEqualToZero = [](int32_t operand1)->bool { return operand1 >= 0; };
	executeBranchZeroOp("bgezal", greaterEqualToZero);
}

void mips::CPU::bgtz()
{
	auto greaterThanZero = [](int32_t operand1)->bool { return operand1 > 0;  };
	executeBranchZeroOp("bgtz", greaterThanZero);
}

void mips::CPU::blez()
{
	auto lessEqualToZero = [](int32_t operand1)->bool { return operand1 <= 0;  };
	executeBranchZeroOp("blez", lessEqualToZero);
}

void mips::CPU::bltz()
{
	auto lessThanZero = [](int32_t operand1)->bool { return operand1 < 0; };
	executeBranchZeroOp("bltz", lessThanZero);
}

void mips::CPU::bltzal()
{
	m_registerFile[cpu_constants::LINK_REGISTER] = m_pc + (cpu_constants::WORD_SIZE_BYTES * 2);
	auto lessThanZero = [](int32_t operand1)->bool { return operand1 < 0; };
	executeBranchZeroOp("bltzal", lessThanZero);
}

void mips::CPU::bne()
{
	auto notEqual = [](uint32_t operand1, uint32_t operand2)->bool { return operand1 != operand2; };
	executeBranchOp("bne", notEqual);
}

void mips::CPU::break_()
{
	raiseException("Breakpoint");
}

void mips::CPU::cfc2()
{
	auto cpFromControl = [this](uint32_t rt, uint32_t rd)->void { m_registerFile[rt] = m_gte.readControlRegister(rd); };
	executeCopOp("cfc2", cpFromControl);
}

void mips::CPU::cop()
{
	uint32_t    copIdx = m_instruction.getCOPIdx();
	std::string warning = "COP" + std::to_string(copIdx) + " is note implemented yet";
	m_debugger->logWarning(warning);
}

void mips::CPU::ctc2()
{
	auto cpToControl = [this](uint32_t rt, uint32_t rd)->void { m_gte.writeContolRegister(rd, m_registerFile[rt]); };
	executeCopOp("ctc2", cpToControl);
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
				m_debugger->logWarning("Instruction tried to divide by 0: ");
			}
			return 0;
	};
	executeHiLoArithmeticOp<int32_t>("div", divideOp);
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
				m_debugger->logWarning("Instruction tried to divide by 0: ");
			}
			return 0;
	};
	executeHiLoArithmeticOp<uint32_t>("divu", divideOp);
}

void mips::CPU::j()
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
	auto loadByteOp = [this](uint32_t addr)->int8_t { return m_bus->readByte(addr); };
	executeLoadOp<int8_t>("lb", loadByteOp);
}

void mips::CPU::lbu()
{
	auto loadByteOp = [this](uint32_t addr)->uint8_t { return m_bus->readByte(addr); };
	executeLoadOp<uint8_t>("lbu", loadByteOp);
}

void mips::CPU::lh()
{
	auto loadHalfwordOp = [this](uint32_t addr)->int16_t { return m_bus->readHalfword(addr); };
	executeLoadOp<int16_t>("lh", loadHalfwordOp);
};

void mips::CPU::lhu()
{
	auto loadHalfwordOp = [this](uint32_t addr)->uint16_t { return m_bus->readHalfword(addr); };
	executeLoadOp<uint16_t>("lhu", loadHalfwordOp);
}

void mips::CPU::lui()
{
	uint32_t rtIdx = m_instruction.getRT();
	Immediate<uint16_t> immediate = Immediate<uint16_t>(m_instruction.getImmediate());
    
	m_registerFile[rtIdx] = immediate.value << 16;

	if (m_context->isDebug())
	{
		Register<uint32_t> rt = Register<uint32_t>(rtIdx, m_registerFile[rtIdx]);
		m_debugger->logGenericRegOp("lui", rt, immediate);
	}
}

void mips::CPU::lw()
{
	auto loadWord = [this](uint32_t addr)->uint32_t { return m_bus->readWord(addr); };
	executeLoadOp<uint32_t>("lw", loadWord);
}

void mips::CPU::lwc2()
{
	auto loadWordCop2 = [this](uint32_t addr, uint32_t rtIdx)->void 
	{
		uint32_t word = m_bus->readWord(addr);
		m_gte.writeDataRegister(rtIdx, word);
	};
	executeCopLoadStoreOp("lwc2", loadWordCop2);
}

void mips::CPU::lwl()
{
	// Have no idea if this is working correctly, it is passing my test, but due to big/little endian difference my understanding of this may be wrong, check other emulators 
	auto adjustLWLOp = [this](uint32_t rt, uint32_t addr, uint32_t word)->uint32_t
	{		
		uint32_t wordOffset = (cpu_constants::WORD_ALIGNED_MASK - (addr & cpu_constants::WORD_ALIGNED_MASK)) * cpu_constants::BYTE_SIZE_BITS;
		uint32_t adjustedWord = word << wordOffset;
		uint32_t rightSideMask = (cpu_constants::WORD_MASK >> (cpu_constants::WORD_SIZE_BITS - wordOffset));
		uint32_t result = adjustedWord | (rt & rightSideMask);
		return result;
	};

	executeLoadWordLROp("lwl", adjustLWLOp);
}

void mips::CPU::lwr()
{
	// Have no idea if this is working correctly, it is passing my test, but due to big/little endian difference my understanding of this may be wrong, check other emulators 
	auto adjustLWROp = [this](uint32_t rt, uint32_t addr, uint32_t word)->uint32_t
	{
		uint32_t wordOffset = (addr & cpu_constants::WORD_ALIGNED_MASK) * cpu_constants::BYTE_SIZE_BITS;
		uint32_t adjustedWord = word >> wordOffset;
		uint32_t leftSideMask = (cpu_constants::WORD_MASK << (cpu_constants::WORD_SIZE_BITS - wordOffset));
		uint32_t result = adjustedWord | (rt & leftSideMask);
		return result;
	};

	executeLoadWordLROp("lwr", adjustLWROp);
}
 
void mips::CPU::mfc()
{	
	auto mfcOp = [this](uint32_t rt, uint32_t rd)->void { m_registerFile[rt] = m_copMap[m_instruction.getCOPIdx()]->readDataRegister(rd); };
	executeCopOp(m_instruction.getCOPIdx() ? "mfc2" : "mfc0", mfcOp);
}

void mips::CPU::mfhi()
{
	auto movFromHiOp = [this]()->uint32_t {	uint32_t rdIdx = m_instruction.getRD(); m_registerFile[rdIdx] = m_hi; return rdIdx; };
	executeMovHiLo("mfhi", movFromHiOp);
}

void mips::CPU::mflo()
{
	auto movFromLoOp = [this]()->uint32_t {	uint32_t rdIdx = m_instruction.getRD(); m_registerFile[rdIdx] = m_lo; return rdIdx; };
	executeMovHiLo("mflo", movFromLoOp);
}

void mips::CPU::mtc()
{
	auto mtcOp = [this](uint32_t rt, uint32_t rd)-> void {m_copMap[m_instruction.getCOPIdx()]->writeDataRegister(rd, m_registerFile[rt]); };
	executeCopOp(m_instruction.getCOPIdx() ? "mt2" : "mtc0", mtcOp);
}

void mips::CPU::mthi()
{
	auto movToHiOp = [this]()->uint32_t { uint32_t rsIdx = m_instruction.getRS(); m_hi = m_registerFile[rsIdx]; return rsIdx; };
	executeMovHiLo("mthi", movToHiOp);
}

void mips::CPU::mtlo()
{
	auto movToLoOp = [this]()->uint32_t { uint32_t rsIdx = m_instruction.getRS(); m_lo = m_registerFile[rsIdx]; return rsIdx; };
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
	executeHiLoArithmeticOp<int32_t>("mult", multiplyOp);
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
	executeHiLoArithmeticOp<uint32_t>("multu", multiplyOp);
}

void mips::CPU::nor()
{
	auto norOp = [](uint32_t operand1, uint32_t operand2)->uint32_t { return ~(operand1 | operand2); };
	executeRegisterArithmeticOp<uint32_t>("nor", norOp, false);
}

void mips::CPU::or_()
{
	auto orOp = [](uint32_t operand1, uint32_t operand2)->uint32_t { return operand1 | operand2; };
	executeRegisterArithmeticOp<uint32_t>("or", orOp, false);
}

void mips::CPU::ori()
{
	auto oriOp = [](uint32_t operand1, uint16_t operand2)->uint32_t { return operand1 | operand2;  };
	executeImmediateArithmeticOp<uint32_t>("ori", oriOp, false);
}

void mips::CPU::sb()
{
	auto sbOp = [this](uint32_t addr, uint32_t data)->void { m_bus->storeByte(addr, data); };
	executeStoreOp("sb", sbOp);
}

void mips::CPU::sh()
{
	auto shOp = [this](uint32_t addr, uint32_t data)->void { m_bus->storeHalfword(addr, data); };
	executeStoreOp("sh", shOp);
}

void mips::CPU::sll()
{
	uint32_t rtIdx = m_instruction.getRT();
	Register<uint32_t>  rt = Register<uint32_t>(rtIdx, m_registerFile[rtIdx]);
	Immediate<uint32_t> sa = Immediate<uint32_t>(m_instruction.getSA());
	executeShiftOp("sll", rt, sa, true);
}

void mips::CPU::sllv()
{
	uint32_t rtIdx = m_instruction.getRT();
	uint32_t rsIdx = m_instruction.getRS();
	Register<uint32_t> rt = Register<uint32_t>(rtIdx, m_registerFile[rtIdx]);
	Register<uint32_t> rs = Register<uint32_t>(rsIdx, m_registerFile[rsIdx] & 0b11111);
	executeShiftOp("sllv", rt, rs, true);
}

void mips::CPU::slt()
{
	auto setOnLessThanOp = [](int32_t rt, int32_t rs)->bool { return rs < rt;  };
	executeRegisterSetOnOp<int32_t>("slt", setOnLessThanOp);
}

void mips::CPU::slti()
{
	auto setOnLessThanImmOp = [](int32_t rs, int32_t immediate)->bool { return rs < immediate; };
	executeImmediateSetOnOp<int32_t>("slti", setOnLessThanImmOp);
}

void mips::CPU::sltiu()
{
	auto setOnLessThanImmOp = [](int32_t rs, int32_t immediate)->bool { return rs < immediate; };
	executeImmediateSetOnOp<uint32_t>("sltiu", setOnLessThanImmOp);
}

void mips::CPU::sltu()
{
	auto setOnLessThanOp = [](int32_t rt, int32_t rs)->bool { return rs < rt;  };
	executeRegisterSetOnOp<uint32_t>("sltu", setOnLessThanOp);
}

void mips::CPU::sra()
{
	uint32_t rtIdx = m_instruction.getRT();
	Register<int32_t>   rt = Register<int32_t>(rtIdx, m_registerFile[rtIdx]);
	Immediate<uint32_t> sa = Immediate<uint32_t>(m_instruction.getSA()); 
	executeShiftOp("sra", rt, sa, false);
}

void mips::CPU::srav()
{
	uint32_t rtIdx = m_instruction.getRT();
	uint32_t rsIdx = m_instruction.getRS();
	Register<int32_t>  rt = Register<int32_t>(rtIdx, m_registerFile[rtIdx]);
	Register<uint32_t> rs = Register<uint32_t>(rsIdx, m_registerFile[rsIdx] & 0b11111);
	executeShiftOp("srav", rt, rs, false);
}

void mips::CPU::srl()
{
	uint32_t rtIdx = m_instruction.getRT();
	Register<uint32_t>  rt = Register<uint32_t>(rtIdx, m_registerFile[rtIdx]);
	Immediate<uint32_t> sa = Immediate<uint32_t>(m_instruction.getSA());
	executeShiftOp("srl", rt, sa, false);
}

void mips::CPU::srlv()
{
	uint32_t rtIdx = m_instruction.getRT();
	uint32_t rsIdx = m_instruction.getRS();
	Register<uint32_t> rt = Register<uint32_t>(rtIdx, m_registerFile[rtIdx]);
	Register<uint32_t> rs = Register<uint32_t>(rsIdx, m_registerFile[rsIdx] & 0b11111);
	executeShiftOp("srlv", rt, rs, false);
}

void mips::CPU::sub()
{
	auto subOp = [](int32_t operand1, int32_t operand2)->int32_t { return operand1 - operand2; };
	executeRegisterArithmeticOp<int32_t>("sub", subOp, true);
}

void mips::CPU::subu()
{
	auto subOp = [](int32_t operand1, int32_t operand2)->int32_t { return operand1 - operand2; };
	executeRegisterArithmeticOp<int32_t>("subu", subOp, false);
}

void mips::CPU::sw()
{
	auto swOp = [this](uint32_t addr, uint32_t data)->void { m_bus->storeWord(addr, data); };
	executeStoreOp("sh", swOp);
}

void mips::CPU::swc2()
{
	auto storeWordCop2Op = [this](uint32_t addr, uint32_t rtIdx)->void
	{
		uint32_t word = m_gte.readDataRegister(rtIdx);
		m_bus->storeWord(addr, word);
	};
	executeCopLoadStoreOp("swc2", storeWordCop2Op);
}

void mips::CPU::swl()
{
	auto adjustWordOp = [](uint32_t memoryWord, uint32_t registerWord, uint32_t addrOffset)->uint32_t
	{
		memoryWord &= cpu_constants::WORD_MASK << ((1 + addrOffset) * cpu_constants::BYTE_SIZE_BITS);
	    registerWord >>= ((3 - addrOffset) * cpu_constants::BYTE_SIZE_BITS);
		return memoryWord | registerWord;
	};
	executeStoreWordLROp("swl", adjustWordOp);
}

void mips::CPU::swr()
{
	auto adjustWord = [](uint32_t memoryWord, uint32_t registerWord, uint32_t addrOffset)->uint32_t
	{
		memoryWord &= cpu_constants::WORD_MASK >> ((4 - addrOffset) * cpu_constants::BYTE_SIZE_BITS);
	    registerWord <<= (addrOffset * cpu_constants::BYTE_SIZE_BITS);
		return memoryWord | registerWord;
	};
	executeStoreWordLROp("swr", adjustWord);
}

void mips::CPU::syscall()
{
	m_debugger->logWarning("SYSCALL is not implement yet");
}

void mips::CPU::xor_()
{
	auto xorOp = [](uint32_t operand1, uint32_t operand2)->uint32_t { return operand1 ^ operand2; };
	executeRegisterArithmeticOp<uint32_t>("xor", xorOp, false);
}

void mips::CPU::xori()
{
	auto xoriOp = [](uint32_t operand1, uint16_t operand2)->uint32_t { return operand1 ^ operand2; };
	executeImmediateArithmeticOp<uint32_t>("xori", xoriOp, false);
}