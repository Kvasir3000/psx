#include <iostream>

#include "../../inc/cpu.h"

mips::CPU::CPU(std::shared_ptr<psx::Context> context) :
	m_context(context),
	m_pc(0)
{
	fillPrimaryOpcodeTable();
	fillSecondaryOpcodeTable();
	fillREGIMMOpcodeTable();
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
		m_pc += cpu_constants::WORD_SIZE;
	}
	else
	{
		executeDelayedJump();
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
	if (m_instruction.isPrimary())
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
	
	m_context->getDebugger()->setPC(m_pc);
	m_instructionCallback();
}

void mips::CPU::executeDelayedJump()
{
	m_pc = m_delaySlot.targetAddress;
	m_context->getDebugger()->setPC(m_pc);
	m_context->getDebugger()->logDelayJump();

	m_delaySlot.status = cpu_constants::DelaySlotState::None;
	m_delaySlot.targetAddress = 0;

}

void mips::CPU::raiseException()
{
	m_context->getDebugger()->logWarning("Exception raised, COP0 is not implemented yet");
}

bool mips::CPU::checkOverflow(int32_t num1, int32_t num2, int32_t result)
{
	if (((num1 > 0) && (num2 > 0) && result <= 0) || ((num1 < 0) && (num2 < 0) && result >= 0))
	{
		return true;
	}
	return false;
}