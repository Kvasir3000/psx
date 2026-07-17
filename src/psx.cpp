#include <iostream>
#include <fstream>

#include "../inc/psx.h"

psx::PlayStation::PlayStation() :
	m_bus(std::make_shared<BUS>()),
	m_context(std::make_shared<Context>(m_bus)),
	m_cpu(mips::CPU(m_context)),
	m_isRunning(true)
{
	testing::TestBuilder testBuilder;
	m_cpu_tests = testBuilder. /*addTestMove().
							  addTestSetOn().
							  addTestArithmeticOps().
		                      addTestStore().
							  addTestLoadWLR().
														  */

						     addTestLoadCOP2().
							 // addTestBranch().
							 // addTestJump().
		                      build();

	readProgramFile();
}

psx::PlayStation::~PlayStation()
{

}

void psx::PlayStation::readProgramFile()
{

}

void psx::PlayStation::run()
{
	for (uint32_t i = 0; i < m_cpu_tests.size(); i++)
	{
		std::string testHeader = "==== TEST [ " + std::to_string(i + 1) + " / " + 
			                     std::to_string(m_cpu_tests.size()) + "]: " + 
								 m_cpu_tests[i]->getName() + " ====\n";
		m_context->getDebugger()->log(testHeader);
		
		m_bus->loadTest(m_cpu_tests[i]);
		m_cpu.setPC(m_cpu_tests[i].get()->getStartAddr());

		while (m_isRunning)
		{
			m_isRunning = m_cpu.emuCycle();
		}

		m_context->getDebugger()->log("\n");
		m_cpu.reset();
		m_bus.get()->reset();
		m_isRunning = true;
	}
}