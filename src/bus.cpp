#include <iostream>
#include <assert.h>
#include <limits> // Temporary 
 
#include "../inc/bus.h"
#include "../inc/constants/cpu_constants.h"
#include "../inc/cpu/opcodes.h"


psx::BUS::BUS()
{ 
	//testLoadOperations();
	//testLoadCOP2();
	//testLoadWLR();
	testMove();
	//testArithmeticOperations();
	//testStore();
	//testSetOn();
	endProgram();
}

void psx::BUS::addInstruction(InstructionDescriptor instruction)
{
	assert(!(instruction.primaryOpcode && instruction.secondaryOpcode), "ERROR: Wrong instruction");
	///assert(!(instruction.immediate && instruction.rd), "ERROR: Wrong instruction");

	uint32_t memoryAddress = instructionCounter * cpu_constants::WORD_SIZE_BYTES;

	uint32_t instructionWord = 0;
	instructionWord |= instruction.secondaryOpcode;
	instructionWord |= static_cast<uint16_t>(instruction.immediate); 
	instructionWord |= static_cast<uint16_t>(instruction.offset);
	instructionWord |= instruction.rd << cpu_constants::RD_OFFSET;
	instructionWord |= instruction.base << cpu_constants::RS_OFFSET;
	instructionWord |= instruction.rs << cpu_constants::RS_OFFSET;
	instructionWord |= instruction.sa << cpu_constants::SA_OFFSET;
	instructionWord |= instruction.rt << cpu_constants::RT_OFFSET; 
	instructionWord |= instruction.regimmType << cpu_constants::REGIMM_TYPE_OFFSET;
	instructionWord |= instruction.copSuboperation << cpu_constants::COP_SUBOPERATION_OFFSET;
	instructionWord |= instruction.copIdx << cpu_constants::COP_INDEX_OFFSET;
	instructionWord |= instruction.copCofun? cpu_constants::COP_OPERATION : 0;
	instructionWord |= instruction.copOp ? cpu_constants::COP_OPCODE : 0;
	instructionWord |= instruction.target;
	instructionWord |= instruction.primaryOpcode << cpu_constants::PRIMARY_OPCODE_OFFSET; 

	memcpy(&m_memory[memoryAddress], &instructionWord, cpu_constants::WORD_SIZE_BYTES);

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

void psx::BUS::initRegister(uint32_t registerIdx, uint32_t value)
{
	InstructionDescriptor instruction = { 0 };
	instruction.primaryOpcode = ADDI;
	instruction.rt = registerIdx;
	instruction.rs = registerIdx;
	instruction.immediate = value;
	addInstruction(instruction);
}

void psx::BUS::mlt(uint32_t rs, uint32_t rt)
{
	InstructionDescriptor instruction = { 0 };
	instruction.secondaryOpcode = MULT;
	instruction.rs = 3;
	instruction.rt = 2;
	addInstruction(instruction);
}

void psx::BUS::mltu(uint32_t rs, uint32_t rt)
{
	InstructionDescriptor instruction = { 0 };
	instruction.secondaryOpcode = MULTU;
	instruction.rs = 3;
	instruction.rt = 2;
	addInstruction(instruction);
}

void psx::BUS::nor(uint32_t rd, uint32_t rs, uint32_t rt)
{
	InstructionDescriptor instruction = { 0 };
	instruction.secondaryOpcode = NOR;
	instruction.rd = rd;
	instruction.rs = rs;
	instruction.rt = rt; 
	addInstruction(instruction);
}

void psx::BUS::or(uint32_t rd, uint32_t rs, uint32_t rt)
{
	InstructionDescriptor instruction = { 0 };
	instruction.secondaryOpcode = OR;
	instruction.rd = rd;
	instruction.rs = rs;
	instruction.rt = rt;
	addInstruction(instruction);
}

void psx::BUS::ori(uint32_t rt, uint32_t rs, uint16_t immediate)
{
	InstructionDescriptor instruction = { 0 };
	instruction.primaryOpcode = ORI;
	instruction.rt = rt;
	instruction.rs = rs;
	instruction.immediate = immediate;
	addInstruction(instruction);
}


void psx::BUS::addi(uint32_t rt, uint32_t rs, int16_t immediate)
{
	InstructionDescriptor instruction = { 0 };
	instruction.primaryOpcode = ADDI;
	instruction.rt = rt;
	instruction.rs = rs;
	instruction.immediate = immediate;
	addInstruction(instruction);
}

void psx::BUS::add(uint32_t rs, uint32_t rt, int32_t rd)
{
	InstructionDescriptor instruction = { 0 };
	instruction.secondaryOpcode = ADD;
	instruction.rt = rs;
	instruction.rs = rt;
	instruction.rd = rd;
	addInstruction(instruction);
}

void psx::BUS::sb(uint32_t base, uint32_t rt, uint16_t offset)
{
	InstructionDescriptor instruction = { 0 };
	instruction.primaryOpcode = SB;
	instruction.base = base; 
	instruction.rt = rt; 
	instruction.offset = offset;
	addInstruction(instruction);
}

void psx::BUS::sh(uint32_t base, uint32_t rt, uint16_t offset)
{
	InstructionDescriptor instruction = { 0 };
	instruction.primaryOpcode = SH;
	instruction.base = base;
	instruction.rt = rt;
	instruction.offset = offset;
	addInstruction(instruction);
}

void psx::BUS::lb(uint32_t rt, uint32_t base, int16_t offset)
{
	InstructionDescriptor instruction = { 0 };
	instruction.primaryOpcode = LB;
	instruction.base = base;
	instruction.offset = offset;
	instruction.rt = rt;
	addInstruction(instruction);
}

void psx::BUS::lh(uint32_t rt, uint32_t base, int16_t offset)
{
	InstructionDescriptor instruction = { 0 };
	instruction.primaryOpcode = LH;
	instruction.base = base;
	instruction.offset = offset;
	instruction.rt = rt;
	addInstruction(instruction);
}

void psx::BUS::lw(uint32_t rt, uint32_t base, int16_t offset)
{
	InstructionDescriptor instruction = { 0 };
	instruction.primaryOpcode = LW;
	instruction.base = base;
	instruction.offset = offset;
	instruction.rt = rt;
	addInstruction(instruction);
}

void psx::BUS::sll(uint32_t rd, uint32_t rt, uint32_t sa)
{
	InstructionDescriptor instruction = { 0 };
	instruction.secondaryOpcode = SLL;
	instruction.rd = rd;
	instruction.rt = rt;
	instruction.sa = sa;
	addInstruction(instruction);
}

void psx::BUS::sllv(uint32_t rd, uint32_t rt, uint32_t rs)
{
	InstructionDescriptor instruction = { 0 };
	instruction.secondaryOpcode = SLLV;
	instruction.rd = rd;
	instruction.rt = rt;
	instruction.rs = rs;
	addInstruction(instruction);
}

void psx::BUS::slt(uint32_t rd, uint32_t rs, uint32_t rt)
{
	InstructionDescriptor instruction = { 0 };
	instruction.secondaryOpcode = SLT;
	instruction.rd = rd;
	instruction.rs = rs;
	instruction.rt = rt; 
	addInstruction(instruction);
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
	return readWord(pc) == 0xDEAD;
}

/* Testing programs */

// 
void psx::BUS::testLoadOperations()
{
	InstructionDescriptor instruction = { 0 };

	initRegister(7, 0x102);

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

	initRegister(2, 18);
	initRegister(9, 20);
	initRegister(12, 0xFE);


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

// Load 314 into COP2 data register 12 (SXY0)
void psx::BUS::testLoadCOP2()
{
	InstructionDescriptor instruction = { 0 };
	instruction.primaryOpcode = LWC2; 
	instruction.base = 0; 
	instruction.rt = 12;
	instruction.offset = 100;
	addInstruction(instruction);

	instruction = { 0 };
	instruction.copSuboperation = MFC;
	instruction.copIdx = 2;
	instruction.rt = 4;
	instruction.rd = 12;
	instruction.copOp = true;
	addInstruction(instruction);

	instruction = { 0 };
	instruction.primaryOpcode = ADDI; 
	instruction.rs = 4;
	instruction.rt = 4; 
	instruction.immediate = -10;
	addInstruction(instruction);

	// 314
	m_memory[100] = 58;
	m_memory[101] = 1;
}

void psx::BUS::testLoadWLR()
{
	InstructionDescriptor instruction = { 0 };
	instruction.primaryOpcode = ADDI;
	instruction.rt = 1;
	instruction.base = 1;
	instruction.offset = 0xEE;
	addInstruction(instruction);

	instruction.primaryOpcode = LWL;
	instruction.rt = 1;
	instruction.base = 2; 
	instruction.offset = 100;
	addInstruction(instruction);

	instruction.primaryOpcode = LWR;
	instruction.rt = 1;
	instruction.base = 2;
	instruction.offset = 101;
	addInstruction(instruction);

	// word1:|AA|BB|CC|DD|
	//       |--|--|--|--|
	m_memory[100] = 0xDD;
	m_memory[101] = 0xCC; 
	m_memory[102] = 0xBB;
	m_memory[103] = 0xAA;
}

void psx::BUS::testMove()
{
	InstructionDescriptor instruction = { 0 };
	initRegister(1, 40);
	initRegister(2, 6);

	instruction = { 0 };
	instruction.secondaryOpcode = DIV;
	instruction.rs = 1; 
	instruction.rd = 3;
	instruction.rt = 2;
	addInstruction(instruction);

	instruction = { 0 };
	instruction.secondaryOpcode = MFHI;
	instruction.rd = 4;
	addInstruction(instruction);

	instruction = { 0 };
	instruction.secondaryOpcode = MFLO;
	instruction.rd = 5; 
	addInstruction(instruction);

	instruction = { 0 };
	instruction.copOp = true;
	instruction.copSuboperation = MTC;
	instruction.copIdx = 0;
	instruction.rt = 5; 
	instruction.rd = 7; 
	addInstruction(instruction);

	instruction = { 0 };
	instruction.copOp = true;
	instruction.copSuboperation = MTC;
	instruction.copIdx = 2;
	instruction.rt = 4;
	instruction.rd = 4;
	addInstruction(instruction);

	instruction = { 0 };
	instruction.copSuboperation = MFC;
	instruction.copIdx = 0;
	instruction.rt = 6;
	instruction.rd = 7;
	instruction.copOp = true;
	addInstruction(instruction);

	instruction = { 0 };
	instruction.copSuboperation = MFC;
	instruction.copIdx = 1;
	instruction.rt = 8;
	instruction.rd = 4;
	instruction.copOp = true;
	addInstruction(instruction);

	instruction = { 0 };
	instruction.secondaryOpcode = ADD;
	instruction.rs = 6;
	instruction.rt = 8;
	instruction.rd = 8;
	addInstruction(instruction);

	instruction = { 0 };
	instruction.secondaryOpcode = MTHI;
	instruction.rs = 8; 
	addInstruction(instruction);

	instruction = { 0 };
	instruction.secondaryOpcode = MTLO;
	instruction.rs = 5;
	addInstruction(instruction);
}


void psx::BUS::testArithmeticOperations()
{
	InstructionDescriptor instruction = { 0 };
	lw(2, 0, 100);
	initRegister(3, 3);
	mlt(2, 3);
	mltu(2, 3);
	initRegister(5, 1);
	initRegister(6, 0);
	nor(7, 5, 6);
	initRegister(8, 2);
	initRegister(9, 1);
	or(9, 9, 8);
	ori(9, 9, 0b1000);
	
	// 2147483647
	m_memory[100] = 0xFF;
	m_memory[101] = 0xFF;
	m_memory[102] = 0xFF;
	m_memory[103] = 0xFF;
}


void psx::BUS::testStore()
{
	initRegister(1, 0x1EEA); 
	sb(2, 1, 104);
	lb(5, 2, 104);
	addNOP();
	addi(5,5,-10);
	sh(6, 1, 104);
	lh(5, 2, 104);
	addNOP();
	addi(5, 5, -10);
	sll(5, 5, 2);
	initRegister(10, 5);
	sllv(5, 5, 10);
}

void psx::BUS::testSetOn()
{
	initRegister(1, 2);
	initRegister(2, 3);
	slt(3, 1, 2);
	slt(3, 2, 1);
}