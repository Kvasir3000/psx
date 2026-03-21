#include <../inc/cpu/cop0.h>

uint32_t mips::COP0::readDataRegister(uint32_t registerIdx)
{
	return m_dataRegisters[registerIdx];
}

void mips::COP0::writeDataRegister(uint32_t registerIdx, uint32_t data)
{
	m_dataRegisters[registerIdx] = data;
}