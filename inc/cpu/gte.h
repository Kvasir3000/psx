#pragma once

#include <stdint.h>
#include <array>

#include "../inc/constants/gte_constants.h"

namespace mips
{
	class GTE
	{
	public:
		GTE();

		uint32_t readDataRegister(uint32_t registerIdx);
		void     writeDataRegister(uint32_t registerIdx, uint32_t data);

		uint32_t readControlRegister(uint32_t registerIdx);
		void     writeContolRegister(uint32_t registerIdx, uint32_t data);



	private:
		std::array<uint32_t, gte_constants::NUMBER_OF_DATA_REGISTERS>    m_dataRegisters;
		std::array<uint32_t, gte_constants::NUMBER_OF_CONTROL_REGISTERS> m_controlRegisters;

	};
}