#include <iostream>
#include <assert.h>
#include <limits> // Temporary 
 
#include "../inc/bus.h"
#include "../inc/constants/cpu_constants.h"
#include "../inc/cpu/opcodes.h"


psx::BUS::BUS()
{ 

}

void psx::BUS::loadTest(testing::Test& test)
{
	const uint8_t* testBin = test.getTestBin();
	memcpy(m_memory, testBin, test.getInstructionCount() * cpu_constants::WORD_SIZE_BYTES);
}

uint8_t psx::BUS::readByte(uint32_t memoryAddress) 
{ 
	return m_memory[memoryAddress]; 
};

uint16_t psx::BUS::readHalfword(uint32_t memoryAddress)
{	
	return (m_memory[memoryAddress] << 8) | m_memory[memoryAddress + 1];
}

uint32_t psx::BUS::readWord(uint32_t memoryAddress)
{
	uint32_t word = 0;

	memcpy(&word, &m_memory[memoryAddress], cpu_constants::WORD_SIZE_BYTES);

	return word;
}

void psx::BUS::storeByte(uint32_t memoryAddress, uint8_t data)
{
	m_memory[memoryAddress] = data;
}


void psx::BUS::storeHalfword(uint32_t memoryAddress, uint16_t data)
{
	m_memory[memoryAddress] = (data >> cpu_constants::BYTE_SIZE_BITS) & cpu_constants::BYTE_MASK;
	m_memory[memoryAddress + 1] = data & cpu_constants::BYTE_MASK;
}

int32_t psx::BUS::getInstructionCounter()
{
	return --instructionCounter;
}

bool psx::BUS::isProgramEnd(uint32_t pc)
{
	return readWord(pc) == cpu_constants::END;
}

void psx::BUS::reset()
{
	memset(m_memory, 0, MEMORY_SIZE);
}

/* Testing programs */

// 
//void psx::BUS::testLoadOperations()
//{
//	InstructionDescriptor instruction = { 0 };
//
//	initRegister(7, 0x102);
//
//	instruction = { 0 };
//	instruction.primaryOpcode = LB;
//	instruction.base = 7;
//	instruction.rt = 2;
//	instruction.offset = -2;
//	addInstruction(instruction);
//
//	instruction = { 0 };
//	instruction.primaryOpcode = LBU;
//	instruction.base = 7;
//	instruction.rt = 9;
//	instruction.offset = -2;
//	addInstruction(instruction);
//
//	initRegister(2, 18);
//	initRegister(9, 20);
//	initRegister(12, 0xFE);
//
//
//	instruction = { 0 };
//	instruction.primaryOpcode = LH;
//	instruction.base = 12;
//	instruction.rt = 12;
//	instruction.offset = 3;
//	addInstruction(instruction);
//
//	instruction = { 0 };
//	instruction.primaryOpcode = LHU;
//	instruction.base = 12;
//	instruction.rt = 12;
//	instruction.offset = 3;
//	addInstruction(instruction);
//
//	instruction = { 0 };
//	instruction.primaryOpcode = ADDI;
//	instruction.rt = 12;
//	instruction.rs = 12;
//	instruction.immediate = 522;
//	addInstruction(instruction);
//	addInstruction(instruction);
//
//	instruction = { 0 };
//	instruction.primaryOpcode = ADDI;
//	instruction.rt = 3;
//	instruction.rs = 3;
//	instruction.immediate = -12;
//	addInstruction(instruction);
//
//	instruction = { 0 };
//	instruction.secondaryOpcode = ADD;
//	instruction.rt = 3;
//	instruction.rs = 2;
//	instruction.rd = 3;
//	addInstruction(instruction);
//
//	instruction = { 0 };
//	instruction.secondaryOpcode = ADD;
//	instruction.rt = 3;
//	instruction.rs = 9;
//	instruction.rd = 3;
//	addInstruction(instruction);
//
//	instruction = { 0 };
//	instruction.primaryOpcode = ADDI;
//	instruction.rt = 13;
//	instruction.rs = 13;
//	instruction.immediate = 0x100;
//	addInstruction(instruction);
//
//	instruction = { 0 };
//	instruction.primaryOpcode = LW; 
//	instruction.base = 13; 
//	instruction.offset = 3;
//	instruction.rt = 19; 
//	addInstruction(instruction); 
//
//	addNOP();
//	addNOP();
//
//	m_memory[0x100] = -4;
//	// 542:
//	/*m_memory[0x101] = 2;
//	m_memory[0x102] = 30*/;
//
//	//-542:
//	m_memory[0x101] = 0xFD;
//	m_memory[0x102] = 0xE2;
//	
//
//	// Data for load word instruction: 3224372736
//	m_memory[0x103] = 0;
//	m_memory[0x104] = 6;
//	m_memory[0x105] = 48; 
//	m_memory[0x106] = 192;
//}

// Load 314 into COP2 data register 12 (SXY0)
//void psx::BUS::testLoadCOP2()
//{
//	InstructionDescriptor instruction = { 0 };
//	instruction.primaryOpcode = LWC2; 
//	instruction.base = 0; 
//	instruction.rt = 12;
//	instruction.offset = 100;
//	addInstruction(instruction);
//
//	instruction = { 0 };
//	instruction.copSuboperation = MFC;
//	instruction.copIdx = 2;
//	instruction.rt = 4;
//	instruction.rd = 12;
//	instruction.copOp = true;
//	addInstruction(instruction);
//
//	addi(4, 4, -10);
//
//	// 314
//	m_memory[100] = 58;
//	m_memory[101] = 1;
//}

//void psx::BUS::testLoadWLR()
//{
//	InstructionDescriptor instruction = { 0 };
//	instruction.primaryOpcode = ADDI;
//	instruction.rt = 1;
//	instruction.base = 1;
//	instruction.offset = 0xEE;
//	addInstruction(instruction);
//
//	instruction.primaryOpcode = LWL;
//	instruction.rt = 1;
//	instruction.base = 2; 
//	instruction.offset = 100;
//	addInstruction(instruction);
//
//	instruction.primaryOpcode = LWR;
//	instruction.rt = 1;
//	instruction.base = 2;
//	instruction.offset = 101;
//	addInstruction(instruction);
//
//	// word1:|AA|BB|CC|DD|
//	//       |--|--|--|--|
//	m_memory[100] = 0xDD;
//	m_memory[101] = 0xCC; 
//	m_memory[102] = 0xBB;
//	m_memory[103] = 0xAA;
//}
//
//void psx::BUS::testStore()
//{
//	initRegister(1, 0x1EEA); 
//	sb(2, 1, 104);
//	lb(5, 2, 104);
//	addNOP();
//	addi(5,5,-10);
//	sh(6, 1, 104);
//	lh(5, 2, 104);
//	addNOP();
//	addi(5, 5, -10);
//	sll(5, 5, 2);
//	initRegister(10, 5);
//	sllv(5, 5, 10);
//}
//
//void psx::BUS::testSetOn()
//{
//	initRegister(1, 2);
//	initRegister(2, 3);
//	slt(3, 1, 2);
//	slt(3, 2, 1);
//}