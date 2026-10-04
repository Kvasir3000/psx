#pragma once
#include <functional>
#include <array>
#include <memory>

#include "../../inc/cpu/opcodes.h"
#include "../../inc/cpu/instruction.h"


namespace mips
{
	template<class T>
	class Decoder
	{
	  using Callback = void(T::*)();
	public:
		Decoder(T* handler);
		Decoder();
		Callback getCallback(Instruction instruction) const;
		bool isInstructionValid(Instruction instruction) const;

	private:
		enum class InstructionType
		{
			REGIM,
			PRIMARY,
			SECONDARY, 
			COP
		};
		std::array<Callback, NUMBER_OF_PRIMARY_OPCODES>   m_primaryOpcodeTable;
		std::array<Callback, NUMBER_OF_SECONDARY_OPCODES> m_secondaryOpcodeTable;
		std::array<Callback, NUMBER_OF_REGIMM_OPCODES>    m_regimmOpcodeTable;
		std::array<Callback, NUMBER_OF_COP_OPCODES>       m_copOpcodeTable;


		void fillPrimaryOpcodeTable();
		void fillSecondaryOpcodeTable();
		void fillREGIMMOpcodeTable();
		void fillCOPOpcodeTable();



		T* m_handler;
	};
}