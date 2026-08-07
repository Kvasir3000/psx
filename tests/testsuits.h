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
			mfc0(a2, 5);
			mfc2(t0, 4);
			add(t0, t0, a2);
			mthi(t0);
			mtlo(a1);
			ctc2(t0, 2);
			cfc2(a1, 2);
			endProgram();
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
			lw(v0, zero, 100);
			initRegister(v1, v1);
			mlt(v0, v1);
			mltu(v0, v1);
			initRegister(a1, 1);
			initRegister(a2, 0);
			nor(a3, a1, a2);
			initRegister(t0, 2);
			initRegister(t1, 1);
			or_(t1, t1, t0);
			ori(t1, t1, 0b1000);
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
			initRegister(t1, 100);

			lwl(t0, t1, 0); 
			nop();
			lwl(t0, t1, 1);
			nop();
			lwl(t0, t1, 2);
			nop();
			lwl(t0, t1, 3);
			nop();

			add(t0, zero, zero);

			lwr(t0, t1, 3);
			nop();
			lwr(t0, t1, 2);
			nop();
			lwr(t0, t1, 1);
			nop();
			lwr(t0, t1, 0);
			nop();

			add(t0, zero, zero);

			lwl(t0, t1, 0);
			lwr(t0, t1, 3);
			nop();
			add(t0, zero, zero);
			lwl(t0, t1, 2);
			lwr(t0, t1, 2);
			nop();
			add(t0, zero, zero);
			lwl(t0, t2, 103);
			lwr(t0, t2, 105);
			nop();
			endProgram();
			//assert(false); // This op is definetly broken, the delay slot should be added, but only after both lwr/lwl or lwl/lwr are called together, no delay slot in between them
			// word1:|AA|BB|CC|DD|
			//       |--|--|--|--|			
			// word2:|AD|AB|EE|FF|
			//       |--|--|--|--|
			m_memory[100] = 0xDD;
			m_memory[101] = 0xCC;
			m_memory[102] = 0xBB;
			m_memory[103] = 0xAA;
			m_memory[104] = 0xFF;
			m_memory[105] = 0xEE;
			m_memory[106] = 0xAB;
			m_memory[107] = 0xAD;
		}
	};

	class TestLoad : public Test
	{
	public: 
		TestLoad() : Test("TEST_LOAD")
		{
			lw(t0, zero, 0x100);
			nop();
			lui(t0, zero, 0xAABB);
			endProgram();
			m_memory[0x100] = 0xEF;
			m_memory[0x101] = 0xBE;			
			m_memory[0x102] = 0xDA;
			m_memory[0x103] = 0xDE;
		};
	};

	class TestLoadByteOperations : public Test
	{
	public:
		TestLoadByteOperations() : Test("TEST_LOAD_BYTE")
		{
			initRegister(t0, 0xF0);
			lb(t1, t0, 3);
			nop();
			lb(t2, t1, 0xF4 - 0x7F);
			nop();
			lbu(t3, t2, 0x80 + 0xF3);
			nop();
			lbu(t4, t3, 0xF4 - 0x7F);
			nop();
			endProgram();	
			m_memory[0xF3] = 0x7F; 
			// 0x80 -> uin8_t = 257 / int8_t = -128 
			m_memory[0xF4] = 0x80;
			m_memory[0xF5] = 0x80;
		}
	};

	class TestLoadHalfwordOperations : public Test
	{
	public:
		TestLoadHalfwordOperations() : Test("TEST_LOAD_HALFWORD")
		{
			initRegister(t0, 0x105);
			lh(t1, t0, -5);
			nop();
			lh(t2, t1, 0);
			nop();
			addi(t2, t2, 0x4000);
			lhu(t3, t2, 0x4100);
			nop();
			lhu(t4, t3, 0);
			nop();

			// 0x101
			m_memory[0x100] = 0x02;
			m_memory[0x101] = 0x01;
			// 0x8000 -> uin16_t = 32768 / int16_t = -=32768
			m_memory[0x102] = 0x00;
			m_memory[0x103] = 0x80;
			endProgram();
		}
	};


	class TestLoadCOP2 : public Test
	{
	public:
		// Load 314 into COP2 data register 12 (SXY0)
		TestLoadCOP2() : Test("TEST_LOAD_COP2")
		{
			initRegister(a0, 20);
			lwc2(12, a0, 100);
			mfc2(t0, 12);
			addi(t0, t0, -10);
			endProgram();

			//// 314
			//m_memory[120] = 58;
			//m_memory[121] = 1;

			//// -23
			m_memory[120] = 0xE9;
			m_memory[121] = 0xFF;
		}
	};

	class TestBranch : public Test
	{
	public: 
		TestBranch() : Test("TEST_BRANCH")
		{
			label("beq");
			{
				initRegister(t0, 0); // i
				initRegister(t1, 4); // i < 4
				initRegister(t2, 1); // var = 1
				initRegister(t3, 2); // const == 2
				label("loop");
				{
					beq(t0, t1, "return_beq");
					nop();
					mlt(t2, t3);   // var *= 2
					mflo(t2);
					addi(t0, t0, 1); // i++
					j("loop");
					nop();
				}
				label("return_beq");
				{
					jr(ra);
					nop();
				}
			}

			label("bne");
			{
				initRegister(t0, 1);
				initRegister(t1, 1);
				label("loop2");
				{
					bne(t0, t1, "return_bne");
					nop();
					addi(t0, t0, -1);
					j("loop2");
					nop();
				}
				label("return_bne");
				{
					jr(ra);
					nop();
				}
			}
			
			label("bgez");
			{
				initRegister(t0, -3);
				label("loop3");
				{
					bgez(t0, 4);
					nop();
					addi(t0, t0, 1);
					j("loop3");
					nop();
				}
				label("increment_t0");
				{
					addi(t0, t0, 1);
				}
				label("loop4");
				{
					bgez(t0, "return_bgez");
					nop();
					addi(t0, t0, 1);
					j("loop4");
					nop();
				}
				label("return_bgez");
				{
					jr(ra);
					nop();
				}
			}


			label("bgezal");
			{
				add(a0, zero, ra); // Save memory addr to main return
				initRegister(t0, 12);
				bgezal(t0, "branch_addr");
				addi(t1, zero, 0);
				bgezal(t0, "branch_addr");
				addi(t1, zero, -12);
				bgezal(t0, "branch_addr"); // this branch should be ingored
				add(ra, zero, a0);
				jr(ra);
				nop();

			};

			label("bgtz");
			{
				initRegister(t0, -4);

				label("loop5");
				{
					bgtz(t0, "program_end");
					addi(t0, t0, 1);
					j("loop5");
					nop();
				};
			};
			
			label("blez");
			{
				initRegister(t0, -5);
				label("loop6");
				{
					blez(t0, "loop6");
					addi(t0, t0, 1);
					j("program_end");
					nop();
				}
			}

			label("bltz");
			{
				initRegister(t0, -5);
				label("loop7");
				{
					bltz(t0, "loop7");
					addi(t0, t0, 1);
					j("program_end");
					nop();
				}
			}

			label("bltzal");
			{
				add(a0, zero, ra); // Save memory addr to main return
				initRegister(t0, -12);
				bltzal(t0, "branch_addr");
				addi(t1, zero, 5);
				bltzal(t0, "branch_addr"); // Should be ingored
				add(ra, zero, a0);
				jr(ra);
				nop();
			}

			label("branch_addr");
			{
				j("set_t0_to_t1");
				nop();
			}

			label("set_t0_to_t1");
			{
				add(t0, zero, t1);
				jr(ra);
				nop();
			}

			label("main");
			{
				jal("bltzal");
				nop();		
				label("program_end");
				{
					endProgram();
				}
			}
		}
	};

	class TestJump : public Test
	{
	public:
		TestJump() : Test("TEST_JUMP")
		{
			label("set_1_if_not_equals");
			{
				initRegister(t0, 12);
				initRegister(t1, 15);
				bne(t0, t1, 2);
				nop();
				or_(t0, t1, t0);
				jr(ra); 
				nop();
			}
		
			label("main");
			{
				jal("set_1_if_not_equals");
				nop();
				initRegister(t0, 23);
				initRegister(t2, 256);
				jal("add_t0_t2");
				nop();
				endProgram();
			}

			label("add_t0_t2");
			{
				add(t3, t0, t2);
				mlt(t3, t3);
				jr(ra);
				nop();
			}
		};
	};

}
