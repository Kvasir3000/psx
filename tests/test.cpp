#include "test.h"
#include <fstream>
#include <filesystem>

namespace testing
{

	Test::Test(std::string name) : m_name(name)
	{

	}

	void Test::addRtBaseOffsetOp(uint32_t opcode, uint32_t rt, uint32_t base, int16_t offset)
	{
		InstructionDescriptor instruction = { 0 };
		instruction.primaryOpcode = opcode;
		instruction.base = base;
		instruction.offset = offset;
		instruction.rt = rt;
		addInstruction(instruction);
	}

	void Test::addRsRtOp(uint32_t opcode, uint32_t rs, uint32_t rt)
	{
		InstructionDescriptor instruction = { 0 };
		instruction.secondaryOpcode = opcode;
		instruction.rs = rs;
		instruction.rt = rt;
		addInstruction(instruction);
	}

	void Test::addRsOp(uint32_t opcode, uint32_t rs)
	{
		InstructionDescriptor instruction = { 0 };
		instruction.secondaryOpcode = opcode;
		instruction.rs = rs;
		addInstruction(instruction);
	}

	void Test::addRsRtImmOp(uint32_t opcode, uint32_t rt, uint32_t rs, int16_t immediate)
	{
		InstructionDescriptor instruction = { 0 };
		instruction.primaryOpcode = opcode;
		instruction.rt = rt;
		instruction.rs = rs;
		instruction.immediate = immediate;
		addInstruction(instruction);
	}

	void Test::addRdRSRtOp(uint32_t opcode, uint32_t rd, uint32_t rs, uint32_t rt)
	{
		InstructionDescriptor instruction = { 0 };
		instruction.secondaryOpcode = opcode;
		instruction.rd = rd;
		instruction.rs = rs;
		instruction.rt = rt;
		addInstruction(instruction);
	}	
	
	void Test::addRdRtSaOp(uint32_t opcode, uint32_t rd, uint32_t rt, uint32_t sa)
	{
		InstructionDescriptor instruction = { 0 };
		instruction.secondaryOpcode = opcode;
		instruction.rd = rd;
		instruction.rt = rt;
		instruction.sa = sa;
		addInstruction(instruction);
	}

	void Test::addRdRtRsOp(uint32_t opcode, uint32_t rd, uint32_t rt, uint32_t rs)
	{
		InstructionDescriptor instruction = { 0 };
		instruction.secondaryOpcode = opcode;
		instruction.rd = rd;
		instruction.rt = rt;
		instruction.rs = rs;
		addInstruction(instruction);
	}

	std::string Test::getName() 
	{
		return m_name; 
	};

	const std::uint8_t* Test::getTestBin()
	{
		return m_memory;
	}

	uint32_t Test::getStartAddr()
	{
		if (m_labelAddrMap.find("main") != m_labelAddrMap.end())
		{
			return m_labelAddrMap["main"];
		};
		return 0; 
	}

	uint32_t Test::getInstructionCount()
	{
		return m_instructionCounter;
	}

	void Test::nop()
	{
		InstructionDescriptor instruction = { 0 };
		addInstruction(instruction);
	}

	void Test::label(std::string name)
	{
		m_labelAddrMap[name] = m_instructionCounter * cpu_constants::WORD_SIZE_BYTES;
	}

	void Test::endProgram()
	{
		InstructionDescriptor instruction = { 0 };
		instruction.immediate = cpu_constants::END;
		addInstruction(instruction);
	}

	void Test::link()
	{
		for (auto entry : m_primaryRelocationEntries)
		{
			auto getOffset = [this, entry]()->uint32_t 
			{
				return (static_cast<int32_t>(m_labelAddrMap[entry.second.name]) - static_cast<int32_t>(entry.first + 4)) >> 2;
			};

			if (m_labelAddrMap.find(entry.second.name) == m_labelAddrMap.end())
			{
				assert(false && "LNK ERROR: uknown symbol");
			}
			if (entry.second.op == JAL)
			{
				jal(m_labelAddrMap[entry.second.name] >> 2, entry.first);
			}
			else if (entry.second.op == J)
			{
				j(m_labelAddrMap[entry.second.name] >> 2, entry.first);
			}
			else if (entry.second.op == BEQ)
			{
				beq(entry.second.rs, entry.second.rt, getOffset(), entry.first);
			}
			else if (entry.second.op == BNE)
			{
				bne(entry.second.rs, entry.second.rt, getOffset(), entry.first);
			}
			else if (entry.second.op == BGTZ)
			{
				bgtz(entry.second.rs, getOffset(), entry.first);
			}
			else if (entry.second.op == BLEZ)
			{
				blez(entry.second.rs, getOffset(), entry.first);
			}
		}

		for (auto entry : m_regimmRelocationEntries)
		{
		   auto getOffset = [this, entry]()->uint32_t
		   {
				return (static_cast<int32_t>(m_labelAddrMap[entry.second.name]) - static_cast<int32_t>(entry.first + 4)) >> 2;
		   };


		   if (entry.second.op == BGEZ)
		   {
			  bgez(entry.second.rs, getOffset(), entry.first);
		   }
		   else if (entry.second.op == BGEZAL)
		   {
			  bgezal(entry.second.rs, getOffset(), entry.first);
		   }
		   else if (entry.second.op == BLTZ)
		   {
			   bltz(entry.second.rs, getOffset(), entry.first);
		   }
		   else if (entry.second.op == BLTZAL)
		   {
			   bltzal(entry.second.rs, getOffset(), entry.first);
		   }
		}
	}

	void Test::generateBinary()
	{
		std::filesystem::path dir = "binFiles/";
		if (!std::filesystem::exists(dir))
		{
			std::filesystem::create_directory(dir);
		}
		std::filesystem::path bin = dir.string() + m_name + ".bin";
		std::fstream binFile(bin, std::fstream::binary | std::ios::out);
		bool opened = binFile.is_open();
		binFile.write((const char*)m_memory, MEMORY_SIZE);
		binFile.close();
	}

	uint32_t Test::getCurrentAddr()
	{
		return m_instructionCounter * cpu_constants::WORD_SIZE_BYTES;
	}

	void Test::addInstruction(InstructionDescriptor instruction, uint32_t memoryAddr)
	{
		assert(!(instruction.primaryOpcode && instruction.secondaryOpcode), "ERROR: Wrong instruction");
		if (memoryAddr == cpu_constants::END)
		{
			memoryAddr = m_instructionCounter * cpu_constants::WORD_SIZE_BYTES;
			m_instructionCounter++;
		}

		uint32_t instructionWord = 0;
		instructionWord |= instruction.secondaryOpcode;
		instructionWord |= static_cast<uint16_t>(instruction.immediate);
		instructionWord |= static_cast<uint16_t>(instruction.offset);
		instructionWord |= instruction.rd << cpu_constants::RD_OFFSET;
		instructionWord |= instruction.base << cpu_constants::RS_OFFSET;
		instructionWord |= instruction.rs << cpu_constants::RS_OFFSET;
		instructionWord |= instruction.sa << cpu_constants::SA_OFFSET;
		instructionWord |= instruction.rt << cpu_constants::RT_OFFSET;
		instructionWord |= instruction.regimmType << cpu_constants::REGIMM_TYPE_OFFSET;
		instructionWord |= instruction.copSuboperation << cpu_constants::COP_SUBOPERATION_OFFSET;
		instructionWord |= instruction.copIdx << cpu_constants::COP_INDEX_OFFSET;
		instructionWord |= instruction.copCofun ? cpu_constants::COP_OPERATION : 0;
		instructionWord |= instruction.copOp ? cpu_constants::COP_OPCODE : 0;
		instructionWord |= instruction.target;
		instructionWord |= instruction.primaryOpcode << cpu_constants::PRIMARY_OPCODE_OFFSET;
		instructionWord |= instruction.regimm ? cpu_constants::REGIMM_OPCODE : 0;

		memcpy(&m_memory[memoryAddr], &instructionWord, cpu_constants::WORD_SIZE_BYTES);
	}


	void Test::initRegister(uint32_t registerIdx, uint32_t value)
	{
		addRsRtImmOp(ADDI, registerIdx, registerIdx, value);
	}

	void Test::mult(uint32_t rs, uint32_t rt)
	{
		addRsRtOp(MULT, rs, rt);
	}

	void Test::multu(uint32_t rs, uint32_t rt)
	{
		addRsRtOp(MULTU, rs, rt);
	}

	void Test::nor(uint32_t rd, uint32_t rs, uint32_t rt)
	{
		addRdRSRtOp(NOR, rd, rs, rt);
	}
	
	void Test::and_(uint32_t rd, uint32_t rs, uint32_t rt)
	{
		addRdRSRtOp(AND, rd, rs, rt);
	}

	void Test::andi(uint32_t rt, uint32_t rs, uint16_t immediate)
	{
		InstructionDescriptor instruction = { 0 };
		instruction.primaryOpcode = ANDI;
		instruction.rt = rt;
		instruction.rs = rs;
		instruction.immediate = immediate;
		addInstruction(instruction);
	}

	void Test::or_(uint32_t rd, uint32_t rs, uint32_t rt)
	{
		addRdRSRtOp(OR, rd, rs, rt);
	}

	void Test::ori(uint32_t rt, uint32_t rs, uint16_t immediate)
	{
		InstructionDescriptor instruction = { 0 };
		instruction.primaryOpcode = ORI;
		instruction.rt = rt;
		instruction.rs = rs;
		instruction.immediate = immediate;
		addInstruction(instruction);
	}

	void Test::addi(uint32_t rt, uint32_t rs, int16_t immediate)
	{
		addRsRtImmOp(ADDI, rt, rs, immediate);
	}	
	
	void Test::addiu(uint32_t rt, uint32_t rs, int16_t immediate)
	{
		addRsRtImmOp(ADDIU, rt, rs, immediate);
	}

	void Test::div(uint32_t rs, uint32_t rt)
	{
	
		addRsRtOp(DIV, rs, rt);
	}

	void Test::divu(uint32_t rs, uint32_t rt)
	{
		addRsRtOp(DIVU, rs, rt);
	}

	void Test::add(int32_t rd, uint32_t rs, uint32_t rt)
	{
		addRdRSRtOp(ADD, rd, rs, rt);
	}	
	
	void Test::addu(int32_t rd, uint32_t rs, uint32_t rt)
	{
		addRdRSRtOp(ADDU, rd, rs, rt);
	}

	void Test::sub(int32_t rd, uint32_t rs, uint32_t rt)
	{
		addRdRSRtOp(SUB , rd, rs, rt);
	}

	void Test::subu(int32_t rd, uint32_t rs, uint32_t rt)
	{
		addRdRSRtOp(SUBU, rd, rs, rt);
	}

	void Test::sb(uint32_t rt, uint32_t base, int16_t offset)
	{
		addRtBaseOffsetOp(SB, rt, base, offset);
	}

	void Test::sh(uint32_t rt, uint32_t base, int16_t offset)
	{
		addRtBaseOffsetOp(SH, rt, base, offset);
	}

	void Test::lb(uint32_t rt, uint32_t base, int16_t offset)
	{
		addRtBaseOffsetOp(LB, rt, base, offset);
	}

	void Test::lbu(uint32_t rt, uint32_t base, int16_t offset)
	{
		addRtBaseOffsetOp(LBU, rt, base, offset);
	}
	
	void Test::lh(uint32_t rt, uint32_t base, int16_t offset)
	{
		addRtBaseOffsetOp(LH, rt, base, offset);
	}
		
	void Test::lhu(uint32_t rt, uint32_t base, int16_t offset)
	{
		addRtBaseOffsetOp(LHU, rt, base, offset);
	}

	void Test::lui(uint32_t rt, uint32_t base, int16_t offset)
	{
		addRtBaseOffsetOp(LUI, rt, base, offset);
	}

	void Test::lw(uint32_t rt, uint32_t base, int16_t offset)
	{
		addRtBaseOffsetOp(LW, rt, base, offset);
	}
	
	void Test::lwl(uint32_t rt, uint32_t base, int16_t offset)
	{
		addRtBaseOffsetOp(LWL, rt, base, offset);
	}

	void Test::lwr(uint32_t rt, uint32_t base, int16_t offset)
	{
		addRtBaseOffsetOp(LWR, rt, base, offset);
	}
	
	void Test::lwc2(uint32_t rt, uint32_t base, int16_t offset)
	{
		addRtBaseOffsetOp(LWC2, rt, base, offset);
	}

	void Test::swc2(uint32_t rt, uint32_t base, int16_t offset)
	{
		addRtBaseOffsetOp(SWC2, rt, base, offset);
	}

	void Test::sll(uint32_t rd, uint32_t rt, uint32_t sa)
	{
		addRdRtSaOp(SLL, rd, rt, sa);
	}	
	
	void Test::sra(uint32_t rd, uint32_t rt, uint32_t sa)
	{
		addRdRtSaOp(SRA, rd, rt, sa);
	}

	void Test::srav(uint32_t rd, uint32_t rt, uint32_t rs)
	{
		addRdRtRsOp(SRAV, rd, rt, rs);
	}

	void Test::srl(uint32_t rd, uint32_t rt, uint32_t sa)
	{
		addRdRtSaOp(SRL, rd, rt, sa);
	}

	void Test::srlv(uint32_t rd, uint32_t rt, uint32_t rs)
	{
		addRdRtRsOp(SRLV, rd, rt, rs);
	}

	void Test::sllv(uint32_t rd, uint32_t rt, uint32_t rs)
	{
		addRdRtRsOp(SLLV, rd, rt, rs);
	}

	void Test::slt(uint32_t rd, uint32_t rs, uint32_t rt)
	{
		addRdRSRtOp(SLT, rd, rs, rt);
	}

	void Test::sltu(uint32_t rd, uint32_t rs, uint32_t rt)
	{
		addRdRSRtOp(SLTU, rd, rs, rt);
	}

	void Test::slti(uint32_t rt, uint32_t rs, int16_t immediate)
	{
		addRsRtImmOp(SLTI, rt, rs, immediate);
	}	
	
	void Test::sltiu(uint32_t rt, uint32_t rs, int16_t immediate)
	{
		addRsRtImmOp(SLTIU, rt, rs, immediate);
	}

	void Test::mfc(uint32_t rt, uint32_t rd, uint32_t copIdx)
	{
		InstructionDescriptor instruction = { 0 };
		instruction.copSuboperation = MFC;
		instruction.copIdx = copIdx;
		instruction.rt = rt;
		instruction.rd = rd;
		instruction.copOp = true;
		addInstruction(instruction);
	}

	void Test::mfc0(uint32_t rt, uint32_t rd)
	{
		mfc(rt, rd, 0);
	}

	void Test::mfc2(uint32_t rt, uint32_t rd)
	{
		mfc(rt, rd, 1);
	}

	void Test::mtc(uint32_t rt, uint32_t rd, uint32_t copIdx)
	{
		InstructionDescriptor instruction = { 0 };
		instruction.copOp = true;
		instruction.copSuboperation = MTC;
		instruction.copIdx = copIdx;
		instruction.rt = rt;
		instruction.rd = rd;
		addInstruction(instruction);
	}

	void Test::mtc0(uint32_t rt, uint32_t rd)
	{
		mtc(rt, rd, 0);
	}

	void Test::mtc2(uint32_t rt, uint32_t rd)
	{
		mtc(rt, rd, 1);
	}

	void Test::mthi(uint32_t rs)
	{
		addRsOp(MTHI, rs);
	};

	void Test::mtlo(uint32_t rs)
	{
		addRsOp(MTLO, rs);

	};

	void Test::mfhi(uint32_t rd)
	{
		InstructionDescriptor instruction = { 0 };
		instruction.secondaryOpcode = MFHI;
		instruction.rd = rd;
		addInstruction(instruction);
	};

	void Test::mflo(uint32_t rd)
	{
		InstructionDescriptor instruction = { 0 };
		instruction.secondaryOpcode = MFLO;
		instruction.rd = rd;
		addInstruction(instruction);
	};

	void Test::ctc2(uint32_t rt, uint32_t rd)
	{
		InstructionDescriptor instruction = { 0 };
		instruction.copOp = true;
		instruction.copSuboperation = CTC;
		instruction.rd = rd;
		instruction.rt = rt;
		instruction.copIdx = 1;
		addInstruction(instruction);
	};

	void Test::cfc2(uint32_t rt, uint32_t rd)
	{
		InstructionDescriptor instruction = { 0 };
		instruction.copOp = true;
		instruction.copSuboperation = CFC;
		instruction.rd = rd;
		instruction.rt = rt;
		instruction.copIdx = 1;
		addInstruction(instruction);
	};

	void Test::beq(uint32_t rs, uint32_t rt, const std::string& label)
	{
		uint32_t addr = getCurrentAddr();
		m_primaryRelocationEntries[addr].name = label;
		m_primaryRelocationEntries[addr].rs = rs;
		m_primaryRelocationEntries[addr].rt = rt;
		m_primaryRelocationEntries[addr].op = BEQ;
		m_instructionCounter++;
	}

	void Test::beq(uint32_t rs, uint32_t rt, int16_t offset, uint32_t relocationAddr)
	{
		InstructionDescriptor instruction = { 0 };
		instruction.primaryOpcode = BEQ;
		instruction.rt = rt;
		instruction.rs = rs;
		instruction.offset = offset;
		if (relocationAddr == cpu_constants::END)
		{
			addInstruction(instruction);
		}
		else
		{
			addInstruction(instruction, relocationAddr);
		}
	}
	
	void Test::bgez(uint32_t rs, const std::string& label)
	{
		uint32_t addr = getCurrentAddr();
		m_regimmRelocationEntries[addr].name = label;
		m_regimmRelocationEntries[addr].op = BGEZ;
		m_regimmRelocationEntries[addr].rs = rs;
		m_instructionCounter++;
	}

	void Test::bgez(uint32_t rs, int16_t offset, uint32_t relocationAddr)
	{
		InstructionDescriptor instruction = { 0 };
		instruction.regimmType = BGEZ;
		instruction.regimm = true;
		instruction.rs = rs; 
		instruction.offset = offset;
		if (relocationAddr == cpu_constants::END)
		{
			addInstruction(instruction);
		}
		else
		{
			addInstruction(instruction, relocationAddr);
		}
	}

	void Test::bgezal(uint32_t rs, int16_t offset, uint32_t relocationAddr)
	{
		InstructionDescriptor instruction = { 0 };
		instruction.regimmType = BGEZAL;
		instruction.regimm = true;
		instruction.rs = rs;
		instruction.offset = offset;
		if (relocationAddr == cpu_constants::END)
		{
			addInstruction(instruction);
		}
		else
		{
			addInstruction(instruction, relocationAddr);
		}
	}

	void Test::bgezal(uint32_t rs, const std::string& label)
	{
		uint32_t addr = getCurrentAddr();
		m_regimmRelocationEntries[addr].name = label;
		m_regimmRelocationEntries[addr].op   = BGEZAL;
		m_regimmRelocationEntries[addr].rs   = rs;
		m_instructionCounter++;
	}

	void Test::bgtz(uint32_t rs, const std::string& label)
	{
		uint32_t addr = getCurrentAddr();
		m_primaryRelocationEntries[addr].name = label;
		m_primaryRelocationEntries[addr].op = BGTZ;
		m_primaryRelocationEntries[addr].rs = rs;
		m_instructionCounter++;
	}

	void Test::bgtz(uint32_t rs, int16_t offset, uint32_t relocationAddr)
	{
		InstructionDescriptor instruction = { 0 };
		instruction.primaryOpcode = BGTZ;
		instruction.rs = rs;
		instruction.offset = offset;
		if (relocationAddr == cpu_constants::END)
		{
			addInstruction(instruction);
		}
		else
		{
			addInstruction(instruction, relocationAddr);
		}
	}

	void Test::blez(uint32_t rs, const std::string& label)
	{
		uint32_t addr = getCurrentAddr();
		m_primaryRelocationEntries[addr].name = label;
		m_primaryRelocationEntries[addr].rs = rs;
		m_primaryRelocationEntries[addr].op = BLEZ;
		m_instructionCounter++;
	}

	void Test::blez(uint32_t rs, int16_t offset, uint32_t relocationAddr)
	{
		InstructionDescriptor instruction = { 0 };
		instruction.primaryOpcode = BLEZ;
		instruction.rs = rs;
		instruction.offset = offset;
		if (relocationAddr == cpu_constants::END)
		{
			addInstruction(instruction);
		}
		else
		{
			addInstruction(instruction, relocationAddr);
		}
	}

	void Test::bltz(uint32_t rs, const std::string& label)
	{
		uint32_t addr = getCurrentAddr();
		m_regimmRelocationEntries[addr].rs = rs;
		m_regimmRelocationEntries[addr].name = label;
		m_regimmRelocationEntries[addr].op = BLTZ;
		m_instructionCounter++;
	}

	void Test::bltz(uint32_t rs, int16_t offset, uint32_t relocationAddr)
	{
		InstructionDescriptor instruction = { 0 };
		instruction.regimmType = BLTZ;
		instruction.rs = rs;
		instruction.offset = offset;
		instruction.regimm = true;
		if (relocationAddr == cpu_constants::END)
		{
			addInstruction(instruction);
		}
		else
		{
			addInstruction(instruction, relocationAddr);
		}
	}

	void Test::bltzal(uint32_t rs, const std::string& label)
	{
		uint32_t addr = getCurrentAddr();
		m_regimmRelocationEntries[addr].name = label;
		m_regimmRelocationEntries[addr].rs = rs;
		m_regimmRelocationEntries[addr].op = BLTZAL;
		m_instructionCounter++;
	}

	void Test::bltzal(uint32_t rs, int16_t offset, uint32_t relocationAddr)
	{
		InstructionDescriptor instruction = { 0 };
		instruction.regimmType = BLTZAL;
		instruction.regimm = true;
		instruction.rs = rs;
		instruction.offset = offset;
		if (relocationAddr == cpu_constants::END)
		{
			addInstruction(instruction);
		}
		else
		{
			addInstruction(instruction, relocationAddr);
		}
	}

	void Test::bne(uint32_t rs, uint32_t rt, const std::string& label)
	{
		uint32_t addr = getCurrentAddr();
		m_primaryRelocationEntries[addr].name = label;
		m_primaryRelocationEntries[addr].rs = rs;
		m_primaryRelocationEntries[addr].rt = rt;
		m_primaryRelocationEntries[addr].op = BNE;
		m_instructionCounter++;
	}

	void Test::bne(uint32_t rs, uint32_t rt, int16_t offset, uint32_t relocationAddr)
	{
		InstructionDescriptor instruction = { 0 };
		instruction.primaryOpcode = BNE;
		instruction.rs = rs;
		instruction.rt = rt;
		instruction.offset = offset;
		if (relocationAddr == cpu_constants::END)
		{
			addInstruction(instruction);
		}
		else
		{
			addInstruction(instruction, relocationAddr);
		}
	}

	void Test::j(uint32_t target, uint32_t relocationAddr)
	{
		InstructionDescriptor instruction = { 0 };
		instruction.primaryOpcode = J;
		instruction.target = target & 0x3FFFFFF;
		if (relocationAddr == cpu_constants::END)
		{
			addInstruction(instruction);
		}
		else
		{
			addInstruction(instruction, relocationAddr);
		}
	}

	void Test::j(const std::string& name)
	{
		m_primaryRelocationEntries[getCurrentAddr()].name = name;
		m_primaryRelocationEntries[getCurrentAddr()].op = J;
		m_instructionCounter++;
	}

	void Test::jal(uint32_t target, uint32_t relocationAddr)
	{
		InstructionDescriptor instruction = { 0 };
		instruction.primaryOpcode = JAL;
		instruction.target = target & 0x3FFFFFF;
		if (relocationAddr == cpu_constants::END)
		{
			addInstruction(instruction);
		}
		else
		{
			addInstruction(instruction, relocationAddr);
		}
	}

	void Test::jal(const std::string& procedureName)
	{
		m_primaryRelocationEntries[getCurrentAddr()].name = procedureName;
		m_primaryRelocationEntries[getCurrentAddr()].op = JAL;

		m_instructionCounter++; 
	}

	void Test::jalr(uint32_t rs, uint32_t rd)
	{
		InstructionDescriptor instruction = { 0 };
		instruction.secondaryOpcode = JALR;
		instruction.rs = rs;
		instruction.rd = rd;
		addInstruction(instruction);
	}	
	
	void Test::jr(uint32_t rs)
	{
		addRsOp(JR, rs);
	}

	void Test::swl(uint32_t rt, uint32_t base, int16_t offset)
	{
		addRtBaseOffsetOp(SWL, rt, base, offset);
	}

	void Test::swr(uint32_t rt, uint32_t base, int16_t offset)
	{
		addRtBaseOffsetOp(SWR, rt, base, offset);
	}

	void Test::syscall()
	{
		InstructionDescriptor instruction = { 0 };
		instruction.secondaryOpcode = SYSCALL;
		addInstruction(instruction);
	}

	void Test::xor_(uint32_t rd, uint32_t rs, uint32_t rt)
	{
		addRdRSRtOp(XOR, rd, rs, rt);
	}

	void Test::xori(uint32_t rt, uint32_t rs, uint16_t immediate)
	{
		addRsRtImmOp(XORI, rs, rt, immediate);
	}
}