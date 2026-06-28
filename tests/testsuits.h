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
			lw(2, 0, 100);
			initRegister(3, 3);
			mlt(2, 3);
			mltu(2, 3);
			initRegister(5, 1);
			initRegister(6, 0);
			nor(7, 5, 6);
			initRegister(8, 2);
			initRegister(9, 1);
			or_(9, 9, 8);
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
			//assert(false); // This op is definetly broken, the delay slot should be added, but only after both lwr/lwl or lwl/lwr are called together, no delay slot in between them
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
			assert(false); // first of all lwc is not loggin cop2 register properly, secondly i do not think that this test is passing
			lwc2(12, 0, 100);
			mfc2(4, 12);
			addi(4, 4, -10);
			endProgram();

			// 314
			m_memory[100] = 58;
			m_memory[101] = 1;
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
