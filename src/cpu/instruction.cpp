#include "../../inc/cpu/instruction.h"
#include "../../inc/constants/cpu_constants.h"
#include "../../inc/cpu/opcodes.h"

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
	auto t = (m_instructionWord & cpu_constants::PRIMARY_OPCODE_MASK) >> cpu_constants::PRIMARY_OPCODE_OFFSET;
	return (m_instructionWord & cpu_constants::PRIMARY_OPCODE_MASK) >> cpu_constants::PRIMARY_OPCODE_OFFSET;
}

bool Instruction::isSecondary()
{
	return (m_instructionWord & cpu_constants::PRIMARY_OPCODE_MASK) == 0;
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

uint32_t Instruction::getBase()
{
	return getRS();
}

uint32_t Instruction::getRD()
{
	return (m_instructionWord & cpu_constants::RD_OPCODE_MASK) >> cpu_constants::RD_OFFSET;
}

uint32_t Instruction::getRT()
{
	return (m_instructionWord & cpu_constants::RT_OPCODE_MASK) >> cpu_constants::RT_OFFSET;
}

uint32_t Instruction::getSA()
{
	return (m_instructionWord & cpu_constants::SA_OPCODE_MASK) >> cpu_constants::SA_OFFSET;
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

bool Instruction::validPrimaryInstruction()
{
	switch (getPrimaryOpcode())
	{
	case ADDI:
	case ADDIU:
	case ANDI:
	case BEQ:
	case BGTZ:
	case BLEZ:
	case BNE:
	case COP0_:
	case COP2:
	case J:
	case JAL:
	case LB:
	case LBU:
	case LH:
	case LHU:
	case LUI:
	case LW:
	case LWC2:
	case LWL:
	case LWR:
	case ORI:
	case SB:
	case SH:
	case SLTI:
	case SLTIU:
	case SWC2:
	case SWL:
	case SW:
	case SWR:
	case XORI:
		return true;
	default:
		return false;
	}
}

bool Instruction::validSecondaryInstruction()
{
	switch (getSecondaryOpcode())
	{
	case ADD:
	case ADDU:
	case AND:
	case BREAK:
	case DIV:
	case DIVU:
	case JALR:
	case JR:
	case MFHI:
	case MFLO:
	case MTHI:
	case MTLO:
	case MULT:
	case MULTU:
	case NOR:
	case OR:
	case SLL:
	case SLLV:
	case SLT:
	case SLTU:
	case SRA:
	case SRAV:
	case SRL:
	case SUB:
	case SUBU:
	case SYSCALL:
	case XOR:
		return true;
	default:
		return false;
	}
}

bool Instruction::validRegimInstruction()
{
	switch (getREGIMMOpcode())
	{
	case BGEZ:
	case BGEZAL:
	case BLTZ:
	case BLTZAL:
		return true;
	default:
		return false;
	}
}

bool Instruction::validCOPInstruction()
{
	switch (getCOPOpcode())
	{
	case CFC:
	case CTC:
	case MFC:
	case MTC:
		return true;
	default:
		return false;
	}
}

bool Instruction::isInstructionValid()
{
	if (isREGIMM())
	{
		return validRegimInstruction();
	}
	else if (isCOP())
	{
		return validCOPInstruction();
	}
	else if (isPrimary())
	{
		return validPrimaryInstruction();
	}
	else if (isSecondary())
	{
		return validSecondaryInstruction();
	}
}