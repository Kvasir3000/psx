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

bool psx::BUS::isProgramEnd(uint32_t pc)
{
	return readWord(pc) == cpu_constants::END;
}

void psx::BUS::reset()
{
	memset(m_memory, 0, MEMORY_SIZE);
}
