#include <iostream>
#include <assert.h>

#include "../../inc/cpu/cpu.h"

mips::CPU::CPU(std::shared_ptr<psx::Context> context) :
	m_context(context),
	m_pc(0)
{
	fillPrimaryOpcodeTable();
	fillSecondaryOpcodeTable();
	fillREGIMMOpcodeTable();
	fillCOPOpcodeTable();

	m_registerFile.fill(0);
}

mips::CPU::CPU()
{
}

mips::CPU::~CPU()
{
}

bool mips::CPU::emuCycle()
{
	if (m_context->getBus()->isProgramEnd(m_pc) && m_delaySlot.status != cpu_constants::DelaySlotState::Execute)
	{
		return false;
	}

	if (m_delaySlot.status != cpu_constants::DelaySlotState::Execute)
	{
		fetchInstruction();
		decodeInstruction();
		executeInstruction();
		m_pc += cpu_constants::WORD_SIZE_BYTES;
	}
	else
	{
		executeDelayedBranch();
	}
	
	if (!m_delayLoads.empty() && m_delayLoads.front().status == cpu_constants::DelaySlotState::Execute)
	{
		executeDelayedLoad();
	}

	return true; 
}

void mips::CPU::fetchInstruction()
{
	uint32_t instructionWord = m_context->getBus()->readWord(m_pc);
	m_instruction = Instruction(instructionWord);
}

void mips::CPU::decodeInstruction()
{
	if (m_instruction.isREGIMM())
	{
		m_instructionCallback = m_regimmOpcodeTable[m_instruction.getREGIMMOpcode()];
	}
	else if (m_instruction.isCOP())
	{
		m_instructionCallback = m_copOpcodeTable[m_instruction.getCOPOpcode()];
	}
	else if (m_instruction.isPrimary())
	{
		m_instructionCallback = m_primaryOpcodeTable[m_instruction.getPrimaryOpcode()];
	}
	else if (m_instruction.isSecondary())
	{
		m_instructionCallback = m_secondaryOpcodeTable[m_instruction.getSecondaryOpcode()];
	}
}

void mips::CPU::executeInstruction()
{
	if (m_delaySlot.status == cpu_constants::DelaySlotState::Pending)
	{
		m_delaySlot.status = cpu_constants::DelaySlotState::Execute;
	}

	if (!m_delayLoads.empty() && m_delayLoads.front().status == cpu_constants::Pending)
	{
		m_delayLoads.front().status = cpu_constants::DelaySlotState::Execute;
	}
	
	if (m_instructionCallback)
	{
		m_context->getDebugger()->logPC(m_pc);
		m_instructionCallback();
		m_instructionCallback = 0;
	}
}

void mips::CPU::executeDelayedBranch()
{
	assert(m_delaySlot.status == cpu_constants::DelaySlotState::Execute && m_delaySlot.targetAddress != 0);

	m_pc = m_delaySlot.targetAddress;

	if (m_context->isDebug())
	{
		m_context->getDebugger()->setPC(m_pc);
		m_context->getDebugger()->logDelayedBranch();
	}


	m_delaySlot.status = cpu_constants::DelaySlotState::None;
	m_delaySlot.targetAddress = 0;

}

void mips::CPU::executeDelayedLoad()
{
	DelayLoad load = m_delayLoads.front();
	assert(load.status == cpu_constants::DelaySlotState::Execute && load.registerIdx != 0);
	

	bool isByte = load.loadSize == cpu_constants::LoadSize::Byte;
	bool isHalfword = load.loadSize == cpu_constants::LoadSize::Halfword;
	bool isWord = load.loadSize == cpu_constants::LoadSize::Word;

	if (isWord || !load.sign)
	{
		m_registerFile[load.registerIdx] = load.data;
	}
	else if (isByte) 
	{
		m_registerFile[load.registerIdx] = static_cast<int8_t>(load.data);
	}
	else if (isHalfword)
	{
		m_registerFile[load.registerIdx] = static_cast<int16_t>(load.data);
	}

	if (m_context->isDebug())
	{
		m_context->getDebugger()->logDelayedLoad(load.registerIdx, load.data, load.sign, isByte, isHalfword);
	}

	m_delayLoads.pop();
}


void mips::CPU::raiseException(std::string exception)
{
	//std::string exception = exceptionType + " exception raised, COP0 is not implemented yet";
	m_context->getDebugger()->logWarning(exception);
}

bool mips::CPU::checkOverflow(int32_t num1, int32_t num2, int32_t result)
{
	if (((num1 > 0) && (num2 > 0) && result <= 0) || ((num1 < 0) && (num2 < 0) && result >= 0))
	{
		return true;
	}
	return false;
}

void mips::CPU::reset()
{
	m_registerFile.fill(0);
	m_hi = 0;
	m_lo = 0;
	m_pc = 0;
}