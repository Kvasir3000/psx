#pragma once

#include <stdint.h>
#include <memory>
#include <functional>
#include <array>
#include <map>
#include <string>

#include "../inc/context.h"
#include "../inc/bus.h"
#include "../inc/opcodes.h"
#include "../inc/cpu_constants.h"
#include "../inc/instruction.h"

#define DEBUG
namespace mips
{
	class CPU
	{
	private:
		std::shared_ptr<psx::Context>                                  m_context;
		std::array<uint32_t, cpu_constants::NUMBER_OF_REGISTERS>       m_registerFile;
		uint32_t                                                       m_pc;

		struct delaytSlot
		{
			cpu_constants::DelaySlotState                              status;
			uint32_t                                                   targetAddress;
		} m_delaySlot;

		
		Instruction                                                    m_instruction;
		std::function<void()>                                          m_instructionCallback;
		std::array<std::function<void()>, NUMBER_OF_PRIMARY_OPCODES>   m_primaryOpcodeTable;
		std::array<std::function<void()>, NUMBER_OF_SECONDARY_OPCODES> m_secondaryOpcodeTable;
		std::array<std::function<void()>, NUMBER_OF_REGIMM_OPCODES>    m_regimmOpcodeTable;

		

		
		void fillPrimaryOpcodeTable();
		void fillSecondaryOpcodeTable();
		void fillREGIMMOpcodeTable();


		void fetchInstruction();
		void decodeInstruction();
		void executeInstruction(); 
		void executeDelayedJump();

		bool checkOverflow(int32_t num1, int32_t num2, int32_t result);

		/* COP0 interface */
		void raiseException();
		
		void executeRegisterTypeArithmeticOp(std::string mnemonic, std::function<int32_t(uint32_t, uint32_t)> arithmeticOp, bool catchException);
		void executeImmediateTypeArithmeticOp(std::string mnemonic, std::function<int32_t(uint32_t, uint16_t)> arithmeticOp, bool catchException);
		void executeBranchOp(std::string mnemonic, std::function<bool(uint32_t, uint32_t)> branchCondition);
		void add();
		void addi();
		void addiu();
		void addu();
		void and_();
		void andi();
		void beq();
		void bgez();

	public:
		CPU(std::shared_ptr<psx::Context> context);
		CPU();
		~CPU();

		bool emuCycle();
	};
}
