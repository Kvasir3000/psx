#include <iostream>
#include <assert.h>
#include <limits> // Temporary 
 
#include "../inc/bus.h"
#include "../inc/cpu_constants.h"
#include "../inc//opcodes.h"


psx::BUS::BUS()
{
/*
*   int i = -10;
*   int b = 0;
*   if (i < 0)
*   {
*      b = 10;
	}
* 
*/

	InstructionDescriptor instruction = { 0 };
	instruction.primaryOpcode = ADDI;
	instruction.immediate = 0;
	instruction.rt = 1;
	instruction.rs = 1;
	addInstruction(instruction);

	instruction = { 0 };
	instruction.primaryOpcode = ADDI;
	instruction.immediate = 0;
	instruction.rt = 2;
	instruction.rs = 2;
	addInstruction(instruction);
	
	instruction = { 0 };
	instruction.primaryOpcode = BNE;
	instruction.rs = 1;
	instruction.rt = 2;
	instruction.offset = 2; 
	addInstruction(instruction);

	instruction = { 0 };
	instruction.primaryOpcode = ADDI;
	instruction.immediate = -2;
	instruction.rs = cpu_constants::LINK_REGISTER;
	instruction.rt = cpu_constants::LINK_REGISTER;
	addInstruction(instruction);

	instruction = { 0 };
	instruction.primaryOpcode = ADDI;
	instruction.immediate = 10; 
	instruction.rs = 2; 
	instruction.rt = 2;
	addInstruction(instruction);

	instruction = { 0 };
	instruction.primaryOpcode = ADDI;
	instruction.immediate = -10;
	instruction.rs = 2;
	instruction.rt = 2;
	addInstruction(instruction);

	instruction = { 0 };
	instruction.secondaryOpcode = BREAK;
	addInstruction(instruction);
	
	endProgram();
}

void psx::BUS::addInstruction(InstructionDescriptor instruction)
{
	assert(!(instruction.primaryOpcode && instruction.secondaryOpcode), "ERROR: Wrong instruction");
	assert(!(instruction.immediate && instruction.rd), "ERROR: Wrong instruction");

	uint32_t memoryAddress = instructionCounter * cpu_constants::WORD_SIZE;

	uint32_t instructionWord = 0;
	instructionWord |= instruction.secondaryOpcode;
	instructionWord |= static_cast<uint16_t>(instruction.immediate); 
	instructionWord |= static_cast<uint16_t>(instruction.offset);
	instructionWord |= instruction.rd << cpu_constants::RD_OFFSET;
	instructionWord |= instruction.rs << cpu_constants::RS_OFFSET; 
	instructionWord |= instruction.rt << cpu_constants::RT_OFFSET; 
	instructionWord |= instruction.regimmType << cpu_constants::REGIMM_TYPE_OFFSET;
	instructionWord |= instruction.primaryOpcode << cpu_constants::PRIMARY_OPCODE_OFFSET; 

	memcpy(&m_memory[memoryAddress], &instructionWord, cpu_constants::WORD_SIZE);

	instructionCounter++;
}

void psx::BUS::addNOP()
{
	InstructionDescriptor instruction = { 0 };
	addInstruction(instruction);
}

void psx::BUS::endProgram()
{
	InstructionDescriptor instruction = { 0 };
	instruction.immediate = 0xDEAD;
	addInstruction(instruction);
}

uint8_t psx::BUS::readByte(uint32_t memoryAddress) 
{ 
	return m_memory[memoryAddress]; 
};

uint32_t psx::BUS::readWord(uint32_t memoryAddress)
{
	uint32_t word = 0;

	memcpy(&word, &m_memory[memoryAddress], cpu_constants::WORD_SIZE);

	return word;
}

int32_t psx::BUS::getInstructionCounter()
{
	return --instructionCounter;
}

bool psx::BUS::isProgramEnd(uint32_t pc)
{
	return readWord(pc) == 0xDEAD;
}

