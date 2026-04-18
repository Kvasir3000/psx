#pragma once
#include <vector>
#include "test.h"

namespace testing
{
	class TestStore : public Test
	{
	public:
		TestStore() : Test("TEST_STORE")
		{
			initRegister(1, 0x1EEA);
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
			initRegister(1, 40);
			initRegister(2, 6);
			div(1, 2);
			mfhi(4);
			mflo(5);
			mtc(5, 7, 0);
			mtc(4, 4, 1);
			mfc(6, 7, 0);
			mfc(8, 4, 1);
			add(6, 8, 8);
			mthi(8);
			mtlo(5);
			cfc2(1, 2);
			endProgram();
			//assert(false); // Test cpc2 + Port the other test cases
		}
	};

	class TestSetOn : public Test
	{
	public:
		TestSetOn() : Test("TEST_SET_ON")
		{
			initRegister(1, 2);
			initRegister(2, 3);
			slt(3, 1, 2);
			slt(3, 2, 1);
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
		//	assert(false); // This op is definetly broken, the delay slot should be added, but only after both lwr/lwl or lwl/lwr are called together, no delay slot in between them
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
			mfc(4, 12, 2);
			addi(4, 4, -10);
			// 314
			m_memory[100] = 58;
			m_memory[101] = 1;
		}
	};
}
