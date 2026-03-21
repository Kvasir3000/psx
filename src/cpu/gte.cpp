#include "../inc/cpu/gte.h"


mips::GTE::GTE()
{
	m_controlRegisters[3] = 314;
}


uint32_t mips::GTE::readDataRegister(uint32_t registerIdx)
{
	return m_dataRegisters[registerIdx];
}

void mips::GTE::writeDataRegister(uint32_t registerIdx, uint32_t data)
{
	m_dataRegisters[registerIdx] = data;
}

uint32_t mips::GTE::readControlRegister(uint32_t registerIdx)
{
	return m_controlRegisters[registerIdx];
}

void mips::GTE::writeContolRegister(uint32_t registerIdx, uint32_t data)
{
	m_controlRegisters[registerIdx] = data;
}