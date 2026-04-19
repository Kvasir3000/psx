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
	m_cpu_tests = testBuilder.addTestMove().
							  addTestSetOn().
							  addTestArithmeticOps().
		                      addTestStore().
							  addTestLoadWLR().
						      addTestLoadCOP2(). 
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
		std::cout << "==== TEST [ " << i + 1 << " / " << m_cpu_tests.size() << "]: " 
			      << m_cpu_tests[i].getName() << " ====\n\n";
		m_bus->loadTest(m_cpu_tests[i]);

		while (m_isRunning)
		{
			m_isRunning = m_cpu.emuCycle();
		}
		std::cout << "\n\n";
		m_cpu.reset();
		m_bus.get()->reset();
		m_isRunning = true;
	}
}