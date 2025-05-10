#include "../../inc/cpu/instruction.h"
#include "../../inc/constants/cpu_constants.h"

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

bool Instruction::isCOP()
{
	/*
       In case bit 25 is set, Coprocessor operation with the given cofunction should be executed, 
	   emulator treats this as a primary opcode operation (COP0, COP2 opcodes)
	*/
	return ((m_instructionWord & cpu_constants::COP_OPCODE_MASK) == cpu_constants::COP_OPCODE) && !(m_instructionWord & cpu_constants::COP_OPERATION);
}

uint32_t Instruction::getCOPOpcode()
{
	return (m_instructionWord & cpu_constants::COP_SUBOPERATION_MASK) >> cpu_constants::COP_SUBOPERATION_OFFSET;
}

uint32_t Instruction::getCOPIdx()
{
	return (m_instructionWord & cpu_constants::COP_INDEX_MASK) >> cpu_constants::COP_INDEX_OFFSET;
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

int32_t Instruction::getJumpTarget()
{
	return (m_instructionWord & cpu_constants::JUMP_TARGET_MASK);
}
