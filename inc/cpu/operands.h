#pragma once
#include <string>
#include "stdint.h"

namespace mips
{
	constexpr std::array<const char*, cpu_constants::NUMBER_OF_REGISTERS> REGISTER_NAMING_MAP =
	{
		"zero", "at", "v0", "v1", "a0", "a1", "a2", "a3", "t0", "t1", "t2",
		"t3",   "t4", "t5", "t6", "t7", "t8", "t9", "s0", "s1", "s2", "s3",
		"s4",   "s5", "s6", "s7", "k0", "k1", "gp", "sp", "fp", "ra"
	};

	template<typename Type>
	struct Register
	{
		Register(uint32_t idx, Type v)
		{
			name = REGISTER_NAMING_MAP[idx];
			value = v;
		};
		Type value;
		std::string name;

		void set_value(Type v) { value = v; }
	};

	template<typename Type>
	struct Immediate
	{
		Immediate(Type v)
		{
			name = std::to_string(v);
			value = v;
		};
		Type value;
		std::string name;
	};

	struct COPRegister
	{
		COPRegister(uint32_t copIdx, uint32_t regIdx, uint32_t v)
		{
			std::string cop_name = (copIdx == 0) ? "COP0" : "GTE";
			name = cop_name + "_r" + std::to_string(regIdx);
			value = v;
		};
		uint32_t value;
		std::string name;
	};

}