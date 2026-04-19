#pragma once 
#include <stdint.h>

namespace mips
{
	class COP
	{
	public: 
		uint32_t readDataRegister(uint32_t registerIdx) { return m_dataRegisters[registerIdx]; };
		void     writeDataRegister(uint32_t registerIdx, uint32_t data) { m_dataRegisters[registerIdx] = data; };
	protected: 
		COP() {};
		std::array<uint32_t, 32>    m_dataRegisters;
	};
}