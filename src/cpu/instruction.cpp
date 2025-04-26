#include "../../inc/instruction.h"
#include "../../inc/cpu_constants.h"

Instruction::Instruction(uint32_t word) :
	m_instructionWord(word)
{

}

Instruction::Instruction()
{

}

bool Instruction::isPrimary()
{
	return (m_instructionWord & cpu_constants::PRIMARY_OPCODE_MASK) > 0;
}

uint32_t Instruction::getPrimaryOpcode()
{
	return (m_instructionWord & cpu_constants::PRIMARY_OPCODE_MASK) >> cpu_constants::PRIMARY_OPCODE_OFFSET;
}

bool Instruction::isSecondary()
{
	return (m_instructionWord & cpu_constants::SECONDARY_OPCODE_MASK) > 0; 
}

uint32_t Instruction::getSecondaryOpcode()
{
	return m_instructionWord & cpu_constants::SECONDARY_OPCODE_MASK;
}

bool Instruction::isREGIMM()
{
	return (m_instructionWord & cpu_constants::PRIMARY_OPCODE_MASK) == cpu_constants::REGIMM_OPCODE;
}

uint32_t Instruction::getREGIMMOpcode()
{
	return (m_instructionWord & cpu_constants::REGIMM_TYPE_MASK) >> cpu_constants::REGIMM_TYPE_OFFSET;
}

uint32_t Instruction::getRS()
{
	return (m_instructionWord & cpu_constants::RS_OPCODE_MASK) >> cpu_constants::RS_OFFSET;
}

uint32_t Instruction::getRD()
{
	return (m_instructionWord & cpu_constants::RD_OPCODE_MASK) >> cpu_constants::RD_OFFSET;
}

uint32_t Instruction::getRT()
{
	return (m_instructionWord & cpu_constants::RT_OPCODE_MASK) >> cpu_constants::RT_OFFSET;
}

int16_t Instruction::getImmediate()
{
	return (m_instructionWord & cpu_constants::IMMEDIATE_MASK); 
}

int16_t Instruction::getOffset()
{
	return (m_instructionWord & cpu_constants::OFFSET_MASK);
}