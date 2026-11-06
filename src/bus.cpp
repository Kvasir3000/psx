#include <iostream>
#include <assert.h>
#include <limits> // Temporary 
 
#include "../inc/bus.h"
#include "../inc/constants/cpu_constants.h"
#include "../inc/cpu/opcodes.h"


psx::BUS::BUS()
{ 
	m_memory.resize(MEMORY_SIZE);
}

void psx::BUS::loadTest(std::shared_ptr<testing::Test>& test, uint32_t testSize)
{
	const uint8_t* testBin = test->getTestBin();
	memcpy(&(*m_memory.begin()), testBin, testSize);
}

uint8_t psx::BUS::readByte(uint32_t addr) 
{ 
	return m_memory[addr]; 
};

uint16_t psx::BUS::readHalfword(uint32_t addr)
{	
	return (m_memory[addr + 1] << 8) | m_memory[addr];
}

uint32_t psx::BUS::readWord(uint32_t addr)
{
	uint32_t word = 0;

	memcpy(&word, &m_memory[addr], cpu_constants::WORD_SIZE_BYTES);

	return word;
}

void psx::BUS::storeByte(uint32_t addr, uint8_t data)
{
	m_memory[addr] = data;
}


void psx::BUS::storeHalfword(uint32_t addr, uint16_t data)
{
	m_memory[addr] = data & cpu_constants::BYTE_MASK;
	m_memory[addr + 1] = (data >> cpu_constants::BYTE_SIZE_BITS);
}

void psx::BUS::storeWord(uint32_t addr, uint32_t data)
{
	m_memory[addr] = data & cpu_constants::BYTE_MASK;
	m_memory[addr + 1] = (data >> cpu_constants::BYTE_SIZE_BITS) & cpu_constants::BYTE_MASK;
	m_memory[addr + 2] = (data >> (cpu_constants::BYTE_SIZE_BITS * 2)) & cpu_constants::BYTE_MASK;
	m_memory[addr + 3] = (data >> (cpu_constants::BYTE_SIZE_BITS * 3));
}

bool psx::BUS::isProgramEnd(uint32_t pc)
{
	return readWord(pc) == cpu_constants::END;
}

void psx::BUS::reset()
{
	memset(&(*m_memory.begin()), 0, MEMORY_SIZE);
}

uint32_t psx::BUS::getMemorySize()
{
	return MEMORY_SIZE;
}
