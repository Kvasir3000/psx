#include <iostream>
#include <assert.h>
#include <limits> // Temporary 
 
#include "../inc/bus.h"
#include "../inc/constants/cpu_constants.h"
#include "../inc/cpu/opcodes.h"


psx::BUS::BUS()
{
	testLoadOperations();
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
	instructionWord |= instruction.base << cpu_constants::RS_OFFSET;
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

uint16_t psx::BUS::readHalfword(uint32_t memoryAddress)
{	
	int16_t a = (m_memory[memoryAddress] << 8) | m_memory[memoryAddress + 1];
	return (m_memory[memoryAddress] << 8) | m_memory[memoryAddress + 1];
}

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

/* Testign programs */

void psx::BUS::testLoadOperations()
{
	InstructionDescriptor instruction = { 0 };

	instruction = { 0 };
	instruction.primaryOpcode = ADDI;
	instruction.rt = 7;
	instruction.rs = 7;
	instruction.immediate = 0x102;
	addInstruction(instruction);

	instruction = { 0 };
	instruction.primaryOpcode = LB;
	instruction.base = 7;
	instruction.rt = 2;
	instruction.offset = -2;
	addInstruction(instruction);

	instruction = { 0 };
	instruction.primaryOpcode = LBU;
	instruction.base = 7;
	instruction.rt = 9;
	instruction.offset = -2;
	addInstruction(instruction);

	instruction = { 0 };
	instruction.primaryOpcode = ADDI;
	instruction.rt = 2;
	instruction.rs = 2;
	instruction.immediate = 18;
	addInstruction(instruction);

	instruction = { 0 };
	instruction.primaryOpcode = ADDI;
	instruction.rt = 9;
	instruction.rs = 9;
	instruction.immediate = 20;
	addInstruction(instruction);

	instruction = { 0 };
	instruction.primaryOpcode = ADDI;
	instruction.rt = 12;
	instruction.rs = 12;
	instruction.immediate = 0xFE;
	addInstruction(instruction);

	instruction = { 0 };
	instruction.primaryOpcode = LH;
	instruction.base = 12;
	instruction.rt = 12;
	instruction.offset = 3;
	addInstruction(instruction);

	instruction = { 0 };
	instruction.primaryOpcode = LHU;
	instruction.base = 12;
	instruction.rt = 12;
	instruction.offset = 3;
	addInstruction(instruction);

	instruction = { 0 };
	instruction.primaryOpcode = ADDI;
	instruction.rt = 12;
	instruction.rs = 12;
	instruction.immediate = 522;
	addInstruction(instruction);
	addInstruction(instruction);

	instruction = { 0 };
	instruction.primaryOpcode = ADDI;
	instruction.rt = 3;
	instruction.rs = 3;
	instruction.immediate = -12;
	addInstruction(instruction);

	instruction = { 0 };
	instruction.secondaryOpcode = ADD;
	instruction.rt = 3;
	instruction.rs = 2;
	instruction.rd = 3;
	addInstruction(instruction);

	instruction = { 0 };
	instruction.secondaryOpcode = ADD;
	instruction.rt = 3;
	instruction.rs = 9;
	instruction.rd = 3;
	addInstruction(instruction);

	instruction = { 0 };
	instruction.primaryOpcode = ADDI;
	instruction.rt = 13;
	instruction.rs = 13;
	instruction.immediate = 0x100;
	addInstruction(instruction);

	instruction = { 0 };
	instruction.primaryOpcode = LW; 
	instruction.base = 13; 
	instruction.offset = 3;
	instruction.rt = 19; 
	addInstruction(instruction); 

	addNOP();
	addNOP();

	m_memory[0x100] = -4;
	// 542:
	/*m_memory[0x101] = 2;
	m_memory[0x102] = 30*/;

	//-542:
	m_memory[0x101] = 0xFD;
	m_memory[0x102] = 0xE2;
	

	// Data for load word instruction: 3224372736
	m_memory[0x103] = 0;
	m_memory[0x104] = 6;
	m_memory[0x105] = 48; 
	m_memory[0x106] = 192;
}