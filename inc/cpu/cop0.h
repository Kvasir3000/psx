#pragma once

#include <array>
#include "../inc/constants/cop0_constants.h"

namespace mips
{
	class COP0 
	{
	public:
		uint32_t readDataRegister(uint32_t registerIdx);
		void     writeDataRegister(uint32_t registerIdx, uint32_t data);
	private:
		std::array<uint32_t, cop0_constants::NUMBER_OF_DATA_REGISTERS>    m_dataRegisters;

	};
}