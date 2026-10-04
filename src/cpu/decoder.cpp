#include "../../inc/cpu/decoder.h"

template<class T>
mips::Decoder<T>::Decoder(T* handler) : m_handler(handler)
{
	fillPrimaryOpcodeTable();
	fillSecondaryOpcodeTable();
	fillREGIMMOpcodeTable();
	fillCOPOpcodeTable();
}

template<class T>
mips::Decoder<T>::Decoder()
{

}

template <class T>
mips::Decoder<T>::Callback mips::Decoder<T>::getCallback(Instruction instruction) const
{
	if (instruction.isREGIMM())
	{
		return m_regimmOpcodeTable[instruction.getREGIMMOpcode()];
	}
	else if (instruction.isCOP())
	{
		return m_copOpcodeTable[instruction.getCOPOpcode()];
	}
	else if (instruction.isPrimary())
	{
		return m_primaryOpcodeTable[instruction.getPrimaryOpcode()];
	}
	else if (instruction.isSecondary())
	{
		return m_secondaryOpcodeTable[instruction.getSecondaryOpcode()];
	}
}

template <class T> 
void mips::Decoder<T>::fillPrimaryOpcodeTable()
{
	m_primaryOpcodeTable[ADDI]  = &T::addi;
	m_primaryOpcodeTable[ADDIU] = &T::addiu;
	m_primaryOpcodeTable[ANDI]  = &T::andi;
	m_primaryOpcodeTable[BEQ]   = &T::beq;
	m_primaryOpcodeTable[BGTZ]  = &T::bgtz;
	m_primaryOpcodeTable[BLEZ]  = &T::blez;
	m_primaryOpcodeTable[BNE]   = &T::bne;
	m_primaryOpcodeTable[COP0_] = &T::cop;
	m_primaryOpcodeTable[COP2]  = &T::cop;
	m_primaryOpcodeTable[J]     = &T::j;
	m_primaryOpcodeTable[JAL]   = &T::jal;
	m_primaryOpcodeTable[LB]    = &T::lb;
	m_primaryOpcodeTable[LBU]   = &T::lbu;
	m_primaryOpcodeTable[LH]    = &T::lh;
	m_primaryOpcodeTable[LHU]   = &T::lhu;
	m_primaryOpcodeTable[LUI]   = &T::lui;
	m_primaryOpcodeTable[LW]    = &T::lw;
	m_primaryOpcodeTable[LWC2]  = &T::lwc2;
	m_primaryOpcodeTable[LWL]   = &T::lwl;
	m_primaryOpcodeTable[LWR]   = &T::lwr;
	m_primaryOpcodeTable[ORI]   = &T::ori;
	m_primaryOpcodeTable[SB]    = &T::sb;
	m_primaryOpcodeTable[SH]    = &T::sh;
	m_primaryOpcodeTable[SLTI]  = &T::slti;
	m_primaryOpcodeTable[SLTIU] = &T::sltiu;
	m_primaryOpcodeTable[SWC2]  = &T::swc2;
	m_primaryOpcodeTable[SWL]   = &T::swl;
	m_primaryOpcodeTable[SWR]   = &T::swr;
	m_primaryOpcodeTable[XORI]  = &T::xori;
}

template <class T>
void mips::Decoder<T>::fillSecondaryOpcodeTable()
{
	m_secondaryOpcodeTable[ADD]     = &T::add;
	m_secondaryOpcodeTable[ADDU]    = &T::addu;
	m_secondaryOpcodeTable[AND]     = &T::and_;
	m_secondaryOpcodeTable[BREAK]   = &T::break_;
	m_secondaryOpcodeTable[DIV]     = &T::div;
	m_secondaryOpcodeTable[DIVU]    = &T::divu;
	m_secondaryOpcodeTable[JALR]    = &T::jalr;
	m_secondaryOpcodeTable[JR]      = &T::jr;
	m_secondaryOpcodeTable[MFHI]    = &T::mfhi;
	m_secondaryOpcodeTable[MFLO]    = &T::mflo;
	m_secondaryOpcodeTable[MTHI]    = &T::mthi;
	m_secondaryOpcodeTable[MTLO]    = &T::mtlo;
	m_secondaryOpcodeTable[MULT]    = &T::mult;
	m_secondaryOpcodeTable[MULTU]   = &T::multu;
	m_secondaryOpcodeTable[NOR]     = &T::nor;
	m_secondaryOpcodeTable[OR]      = &T::or_;
	m_secondaryOpcodeTable[SLL]     = &T::sll;
	m_secondaryOpcodeTable[SLLV]    = &T::sllv;
	m_secondaryOpcodeTable[SLT]     = &T::slt;
	m_secondaryOpcodeTable[SLTU]    = &T::sltu;
	m_secondaryOpcodeTable[SRA]     = &T::sra;
	m_secondaryOpcodeTable[SRAV]    = &T::srav;
	m_secondaryOpcodeTable[SRL]     = &T::srl;
	m_secondaryOpcodeTable[SRLV]    = &T::srlv;
	m_secondaryOpcodeTable[SUB]     = &T::sub;
	m_secondaryOpcodeTable[SUBU]    = &T::subu;
	m_secondaryOpcodeTable[SYSCALL] = &T::syscall;
	m_secondaryOpcodeTable[XOR]     = &T::xor_;
}

template <class T>
void mips::Decoder<T>::fillREGIMMOpcodeTable()
{
	m_regimmOpcodeTable[BGEZ]   = &T::bgez;
	m_regimmOpcodeTable[BGEZAL] = &T::bgezal;
	m_regimmOpcodeTable[BLTZ]   = &T::bltz;
	m_regimmOpcodeTable[BLTZAL] = &T::bltzal;
}

template <class T>
void mips::Decoder<T>::fillCOPOpcodeTable()
{
	m_copOpcodeTable[CFC] = &T::cfc2;
	m_copOpcodeTable[CTC] = &T::ctc2;
	m_copOpcodeTable[MFC] = &T::mfc;
	m_copOpcodeTable[MTC] = &T::mtc;
}