#pragma once

enum PrimaryOpcodes
{
	SPECIAL,
	REGIMM,
	J,
	JAL,
	BEQ, 
	BNE,
	BLEZ,
	BGTZ,
	ADDI,
	ADDIU,
	SLTI,
	SLTIU,
	ANDI,
	ORI,
	XORI,
	LUI,
	COP0,
	COP1,
	COP2,
	COP3,
	LB = 0x20,
	LH,
	LWL,
	LW,
	LBU,
	LHU,
	LWR,
	SB,
	SH,
	SWL,
	SW,
	SWR  = 0x2E,
	LWC0 = 0x30,
	LWC1,
	LWC2,
	LWC3,
	SWC0 = 0x38,
	SWC1,
	SWC2,
	SWC3,
	NUMBER_OF_PRIMARY_OPCODES
};

enum SecondaryOpcodes
{
	SLL,
	SRL = 0x2,
	SRA,
	SLLV,
	SRLV = 0x6,
	SRAV,
	JR,
	JARL,
	SYSCALL = 0xC,
	BREAK,
	MFHI = 0x10,
	MTHI,
	MFLO,
	MTLO,
	MULT,
	MULTU,
	DIV,
	DIVU,
	ADD = 0x20, 
	ADDU,
	SUB,
	SUBU,
	AND, 
	OR,
	XOR,
	NOR,
	SLT = 0x2A,
	SLTU,
	NUMBER_OF_SECONDARY_OPCODES,
};

enum RegisterImmediateOpcodes
{
	BLTZ, 
	BGEZ,
	BLTZL,
	BGZEL,
	BLTZAL,
	BGEZAL,
	BLTZALL,
	BGEZALL,
	NUMBER_OF_REGIMM_OPCODES
};

enum COPOpcodes
{
	MFC, 
	CFC = 0x2,
	MTC = 0x4,
	CTC = 0x6,
	// BCT and BCF are not implemented
	NUMBER_OF_COP_OPCODES
};