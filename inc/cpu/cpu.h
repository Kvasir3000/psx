#pragma once

#include <stdint.h>
#include <memory>
#include <functional>
#include <array>
#include <map>
#include <string>

#include "../inc/context.h"
#include "../inc/bus.h"
#include "../inc/cpu/opcodes.h"
#include "../inc/constants/cpu_constants.h"
#include "../inc/cpu/instruction.h"
#include "../inc/cpu/gte.h"

namespace mips
{
	class CPU
	{
	public:
		CPU(std::shared_ptr<psx::Context> context);
		CPU();
		~CPU();

		bool emuCycle();

	private:
		std::shared_ptr<psx::Context>                                  m_context;
		std::array<uint32_t, cpu_constants::NUMBER_OF_REGISTERS>       m_registerFile; 
		uint32_t                                                       m_hi; 
		uint32_t                                                       m_lo;
		uint32_t                                                       m_pc;

		struct delaytSlot
		{
			cpu_constants::DelaySlotState                              status;
			uint32_t                                                   targetAddress;
		} m_delaySlot, m_delayLoad;

		
		Instruction                                                    m_instruction;
		std::function<void()>                                          m_instructionCallback;
		std::array<std::function<void()>, NUMBER_OF_PRIMARY_OPCODES>   m_primaryOpcodeTable;
		std::array<std::function<void()>, NUMBER_OF_SECONDARY_OPCODES> m_secondaryOpcodeTable;
		std::array<std::function<void()>, NUMBER_OF_REGIMM_OPCODES>    m_regimmOpcodeTable;
		std::array<std::function<void()>, NUMBER_OF_COP_OPCODES>       m_copOpcodeTable;

		

		
		void fillPrimaryOpcodeTable();
		void fillSecondaryOpcodeTable();
		void fillREGIMMOpcodeTable();
		void fillCOPOpcodeTable();



		void fetchInstruction();
		void decodeInstruction();
		void executeInstruction(); 
		void executeDelayedBranch();

		bool checkOverflow(int32_t num1, int32_t num2, int32_t result);

		/* COP0 interface */
		void raiseException(std::string exceptionType);

		GTE m_gte;


		struct ArithmeticOpFlags
		{
			bool catchException;
			bool isMultiplicative;
			bool isSigned;
		};
		
		void executeRegisterTypeArithmeticOp(std::string mnemonic, std::function<int32_t(uint32_t, uint32_t)> arithmeticOp, ArithmeticOpFlags flags);
		void executeImmediateTypeArithmeticOp(std::string mnemonic, std::function<int32_t(uint32_t, uint16_t)> arithmeticOp, bool catchException);
		void executeBranchOp(std::string mnemonic, std::function<bool(uint32_t, uint32_t)> branchCondition, bool compareToZero);
		void executeJump(std::string mnemonic);
		void excecuteJumpRegister(std::string mnemonic);

		void add();
		void addi();
		void addiu();
		void addu();
		void and_();
		void andi();
		void beq();
		void bgez();
		void bgezal();
		void bgtz();
		void blez();
		void bltz();
		void bltzal();
		void bne();
		void break_();
		void cfc2();
		void cop();
		void ctc2();
		void div();
		void divu();
		void jump();
		void jal();
		void jalr();
		void jr();
		void lb();
	};
}
