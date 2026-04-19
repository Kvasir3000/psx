#pragma once
#include <vector>
#include "test.h"
#include "registermap.h"

namespace testing
{
	class TestStore : public Test
	{
	public:
		TestStore() : Test("TEST_STORE")
		{
			initRegister(at, 0x1EEA);
			sb(2, 1, 104);
			lb(5, 2, 104);
			nop();
			addi(5, 5, -10);
			sh(6, 1, 104);
			lh(5, 2, 104);
			nop();
			addi(5, 5, -10);
			sll(5, 5, 2);
			initRegister(10, 5);
			sllv(5, 5, 10);
			endProgram();
		}
	};

	class TestMove : public Test
	{
	public:
		TestMove() : Test("TEST_MOVE")
		{
			initRegister(at, 40);
			initRegister(v0, 6);
			div(at, v0);
			mfhi(a0);
			mflo(a1);
			mtc0(a3, a1);
			mtc2(a0, a0);
			mfc0(a2, 7);
			mfc2(t0, 4);
			add(t0, t0, a2);
			mthi(t0);
			mtlo(a1);
			ctc2(t0, 2);
			cfc2(a1, 2);
			endProgram();
			assert(false); // Test cpc2 + Port the other test cases
		}
	};

	class TestSetOn : public Test
	{
	public:
		TestSetOn() : Test("TEST_SET_ON")
		{
			initRegister(at, 2);
			initRegister(v0, 3);
			slt(v1, at, v0);
			slt(v1, v0, at);
			endProgram();
		}
	};


	class TestArithmeticOps : public Test
	{
	public: 
		TestArithmeticOps() : Test("TEST_ARITHMETIC_OPS")
		{
			lw(2, 0, 100);
			initRegister(3, 3);
			mlt(2, 3);
			mltu(2, 3);
			initRegister(5, 1);
			initRegister(6, 0);
			nor(7, 5, 6);
			initRegister(8, 2);
			initRegister(9, 1);
			_or(9, 9, 8);
			ori(9, 9, 0b1000);
			endProgram();

			// 2147483647
			m_memory[100] = 0xFF;
			m_memory[101] = 0xFF;
			m_memory[102] = 0xFF;
			m_memory[103] = 0xFF;
		}
	};
	

	class TestLoadWLR : public Test
	{
	public:
		TestLoadWLR() : Test("TEST_LOAD_WLR")
		{

			lwl(1, 2, 100);
			lwr(1, 2, 100);
			endProgram();
			assert(false); // This op is definetly broken, the delay slot should be added, but only after both lwr/lwl or lwl/lwr are called together, no delay slot in between them
			// word1:|AA|BB|CC|DD|
			//       |--|--|--|--|
			m_memory[100] = 0xDD;
			m_memory[101] = 0xCC;
			m_memory[102] = 0xBB;
			m_memory[103] = 0xAA;
		}
	};

	class TestLoadOperations : public Test
	{
	public:
		TestLoadOperations() : Test("TEST_LOAD_OPERATIONS")
		{
			initRegister(7, 0x102);
			lb(2, 7, -2);
			lbu(7, 9, -2);
			initRegister(2, 18);
			initRegister(9, 20);
			initRegister(12, 0xFE);
			lh(12, 12, 3);
			lbu(12, 12, 3);
			addi(12, 12, 522);
			addi(12, 12, 522);
			addi(3, 3, -12);
			add(2, 3, 3);
			add(9, 3, 3);
			addi(13, 13, 0x100);
			lw(19, 13, 3);
			nop();
			nop();
			endProgram();

			m_memory[0x100] = -4;
			// 542:
			/*m_memory[0x101] = 2;
			m_memory[0x102] = 30*/;

			//-542:
			m_memory[0x101] = 0xFD;
			m_memory[0x102] = 0xE2;

			// Data for load word instruction: 3224372736
			m_memory[0x103] = 0;
			m_memory[0x104] = 6;
			m_memory[0x105] = 48;
			m_memory[0x106] = 192;
		}
	};

	class TestLoadCOP2 : public Test
	{
	public:
		// Load 314 into COP2 data register 12 (SXY0)
		TestLoadCOP2() : Test("TEST_LOAD_COP2")
		{
			lwc2(12, 0, 100);
			mfc2(4, 12);
			addi(4, 4, -10);
			endProgram();

			// 314
			m_memory[100] = 58;
			m_memory[101] = 1;
		}
	};
}
