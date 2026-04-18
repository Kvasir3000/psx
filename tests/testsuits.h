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
			assert(false); // This op is definetly broken, the delay slot should be added, but only after both lwr/lwl or lwl/lwr are called together, no delay slot in between them
			// word1:|AA|BB|CC|DD|
			//       |--|--|--|--|
			m_memory[100] = 0xDD;
			m_memory[101] = 0xCC;
			m_memory[102] = 0xBB;
			m_memory[103] = 0xAA;
		}
	};
}