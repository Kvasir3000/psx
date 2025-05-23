#include <iostream>
#include <assert.h>
#include <limits> // Temporary 
 
#include "../inc/bus.h"
#include "../inc/constants/cpu_constants.h"
#include "../inc/cpu/opcodes.h"


psx::BUS::BUS()
{
	InstructionDescriptor instruction = { 0 };
	instruction.primaryOpcode = ADDI;
	instruction.immediate = 1;
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
	instruction.primaryOpcode = ADDI;
	instruction.immediate = 24;
	instruction.rt = 3;
	instruction.rs = 3;
	addInstruction(instruction);

	instruction = { 0 };
	instruction.secondaryOpcode = JR;
	instruction.rs = 3;
	addInstruction(instruction);

	//
	//instruction = { 0 };
	//instruction.primaryOpcode = BNE;
	//instruction.rs = 1;
	//instruction.rt = 2;
	//instruction.offset = 2; 
	//addInstruction(instruction);

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
	instruction.primaryOpcode = COP2; 
	instruction.copSuboperation = CFC;
	instruction.rd = 3; 
	instruction.rt = 10; 
	addInstruction(instruction);

	instruction = { 0 };
	instruction.primaryOpcode = COP2;
	instruction.copCofun = true;
	addInstruction(instruction);

	instruction = { 0 };
	instruction.primaryOpcode = COP0;
	instruction.copCofun = true;
	addInstruction(instruction);

	instruction = { 0 };
	instruction.primaryOpcode = COP2; 
	instruction.copSuboperation = CTC; 
	instruction.rt = 10;
	instruction.rd = 10; 
	addInstruction(instruction);
	
	instruction = { 0 };
	instruction.primaryOpcode = J;
	instruction.target = 0xD;
	addInstruction(instruction);

	instruction = { 0 };
	instruction.primaryOpcode = ADDI; 
	instruction.rt = 15; 
	instruction.rs = 15; 
	instruction.immediate = -3; 
	addInstruction(instruction);

	instruction = { 0 };
	instruction.secondaryOpcode = DIV;
	instruction.rt = 15;
	instruction.rs = 10;
	addInstruction(instruction);

	instruction = { 0 };
	instruction.secondaryOpcode = DIVU;
	instruction.rt = 15;
	instruction.rs = 10; 
	addInstruction(instruction);

	instruction = { 0 };
	instruction.secondaryOpcode = BREAK;
	addInstruction(instruction);
	
	endProgram();
}

void psx::BUS::addInstruction(InstructionDescriptor instruction)
{
	assert(!(instruction.primaryOpcode && instruction.secondaryOpcode), "ERROR: Wrong instruction");
	///assert(!(instruction.immediate && instruction.rd), "ERROR: Wrong instruction");

	uint32_t memoryAddress = instructionCounter * cpu_constants::WORD_SIZE;

	uint32_t instructionWord = 0;
	instructionWord |= instruction.secondaryOpcode;
	instructionWord |= static_cast<uint16_t>(instruction.immediate); 
	instructionWord |= static_cast<uint16_t>(instruction.offset);
	instructionWord |= instruction.rd << cpu_constants::RD_OFFSET;
	instructionWord |= instruction.rs << cpu_constants::RS_OFFSET; 
	instructionWord |= instruction.rt << cpu_constants::RT_OFFSET; 
	instructionWord |= instruction.regimmType << cpu_constants::REGIMM_TYPE_OFFSET;
	instructionWord |= instruction.copSuboperation << cpu_constants::COP_SUBOPERATION_OFFSET;
	instructionWord |= instruction.copCofun? cpu_constants::COP_OPERATION : 0;
	instructionWord |= instruction.target;
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

