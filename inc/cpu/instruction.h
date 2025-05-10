#pragma once

#include <stdint.h>

class Instruction
{
public:
	Instruction(uint32_t word);
	Instruction();


	bool     isPrimary();
	uint32_t getPrimaryOpcode();

	bool     isSecondary();
	uint32_t getSecondaryOpcode();

	bool     isREGIMM();
	uint32_t getREGIMMOpcode();

	bool     isCOP();
	uint32_t getCOPOpcode();
	uint32_t getCOPIdx();

	uint32_t getRS();
	uint32_t getRT();
	uint32_t getRD();
	int16_t  getImmediate();
	int16_t  getOffset();
	int32_t  getJumpTarget();


private: 
	uint32_t m_instructionWord; 
};