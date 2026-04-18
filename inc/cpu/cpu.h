#pragma once

#include <stdint.h>
#include <memory>
#include <functional>
#include <array>
#include <map>
#include <string>
#include <queue>

#include "../inc/context.h"
#include "../inc/bus.h"
#include "../inc/cpu/opcodes.h"
#include "../inc/constants/cpu_constants.h"
#include "../inc/cpu/instruction.h"
#include "../inc/cpu/gte.h"
#include "../inc/cpu/cop0.h"

namespace mips
{
	class CPU
	{
	public:
		CPU(std::shared_ptr<psx::Context> context);
		CPU();
		~CPU();

		bool emuCycle();
		void reset();

	private:
	
		template<typename Type>
		struct Register
		{
			Register(uint32_t idx, Type v)
			{ 
				name = "r" + std::to_string(idx);
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

		std::shared_ptr<psx::Context>                                  m_context;
		std::array<uint32_t, cpu_constants::NUMBER_OF_REGISTERS>       m_registerFile; 
		uint32_t                                                       m_hi; 
		uint32_t                                                       m_lo;
		uint32_t                                                       m_pc;
		
		struct DelaySlot
		{
			cpu_constants::DelaySlotState                              status;
			uint32_t                                                   targetAddress;
		} m_delaySlot;

		struct DelayLoad
		{
			cpu_constants::DelaySlotState                              status;
			cpu_constants::LoadSize                                    loadSize;
			uint32_t                                                   registerIdx;
			uint32_t                                                   data;
			bool                                                       sign; 
		};
		std::queue<DelayLoad>                                          m_delayLoads;

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
		void executeDelayedLoad();

		bool checkOverflow(int32_t num1, int32_t num2, int32_t result);

		/* COP0 interface */
		void raiseException(std::string exceptionType);

		GTE  m_gte;
		COP0 m_cop0;


		struct ArithmeticOpFlags
		{
			bool catchException;
			bool isMultiplicative;
		};

		struct LoadOpFlags
		{
			cpu_constants::LoadSize size;
			bool isSigned;
			uint32_t alignMask = 0;
			bool isCop2 = false;
		};
		template<typename Type, typename ArithmeticOp>
		void executeRegisterArithmeticOp(const std::string& mnemonic, const ArithmeticOp& arithmeticOp, const ArithmeticOpFlags& opFlags);
		template<typename Type, typename ArithmeticOp>
		void executeImmediateArithmeticOp(const std::string&  mnemonic, const ArithmeticOp& arithmeticOp, bool catchException);
		void executeBranchOp(const std::string& mnemonic, const std::function<bool(uint32_t, uint32_t)>& branchCondition, bool compareToZero);
		void executeJumpOp(const std::string& mnemonic);
		void excecuteJumpRegisterOp(const std::string& mnemonic);
		void executeLoadOp(const std::string& mnemonic, const std::function<uint32_t(uint32_t)>& readOp, const LoadOpFlags& opFlags);
		void executeLoadWordLROp(const std::string& mnemonic, const std::function<uint32_t(uint32_t, uint32_t, uint32_t)>& adjustWord);
		template<typename MovOp>
		void executeMovHiLo(const std::string& mnemonic, const MovOp& movOp);
		void executeStoreOp(const std::string& mnemonic, const std::function<void(uint32_t, uint32_t)>& storeOp);
		void executeShiftOp(const std::string& mnemonic, const std::function<uint32_t(uint32_t, uint32_t&)>& shiftOp, bool isLogical);
		template<typename Type, typename SetOp>
		void executeRegisterSetOnOp(const std::string& mnemonic, const SetOp& setOp);
		template<typename Type, typename SetOp>
		void executeImmediateSetOnOp(const std::string& mnemonic, const SetOp& setOp);
		/*void executeRegisterSetOnOp(const std::string& mnemonic, const std::function<bool(uint32_t, uint32_t)>& setOperation);
		void executeImmediateSetOnOp(const std::string& mnemonic, const std::function<bool(uint32_t, uint32_t)>& setOperation);*/


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
		void lbu();
		void lh();
		void lhu();
		void lui();
		void lw();
		void lwc2();
		void lwl();
		void lwr();
		void mfc();
		void mfhi();
		void mflo();
		void mtc();
		void mthi();
		void mtlo();
		void mult();
		void multu();
		void nor();
		void _or();
		void ori();
		void sb();
		void sh();
		void sll();
		void sllv();
		void slt();
		void slti();
	};
}
