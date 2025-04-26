#pragma once

#include <stdint.h>

class Instruction
{
private: 
	uint32_t m_instructionWord; 
public:
	Instruction(uint32_t word);
	Instruction();


	bool     isPrimary();
	uint32_t getPrimaryOpcode();

	bool     isSecondary();
	uint32_t getSecondaryOpcode();

	bool     isREGIMM();
	uint32_t getREGIMMOpcode();


	uint32_t getRS();
	uint32_t getRT();
	uint32_t getRD();
	int16_t  getImmediate();
	int16_t  getOffset();
};