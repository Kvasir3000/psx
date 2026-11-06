#include <iostream>
#include <fstream>

#include "../inc/psx.h"
#include "../disassembly/disassembly.h"

psx::PlayStation::PlayStation() :
	m_bus(std::make_shared<BUS>()),
	m_debugger(std::make_shared<Debugger>("trace.txt")),
	m_context(std::make_shared<Context>(m_bus, m_debugger)),
	m_cpu(mips::CPU(m_context)),
	m_isRunning(true)
{
	testing::TestBuilder testBuilder;
	m_cpu_tests = testBuilder.addTestMove().
							  addTestSetOn().
							  addTestArithmeticOps().
							  addTestLoadWLR().
							  addTestStore().
							  addTestLoadByteOperations().
							  addTestLoadHalfwordOperations().
							  addTestLoad().
							  addTestLoadCOP2().
							  addTestBranch().
							  addTestJump().
							  addTestArithmeticOps().
							  addTestStoreWLR().
		                      build();

	readProgramFile();
	
}

psx::PlayStation::~PlayStation()
{

}

void psx::PlayStation::readProgramFile()
{
	for (uint32_t i = 0; i < m_cpu_tests.size(); i++)
	{
		disassemble(m_cpu_tests[i]->getName());
	}
}

void psx::PlayStation::disassemble(const std::string& name)
{
	std::filesystem::path binPath = "bios/" + name + ".bin";
	std::filesystem::create_directory("disFiles");
	std::filesystem::path outPath = "disFiles/" + name + ".dis";
	mips::Disassembly dis(binPath, outPath);
	dis.run();
}

void psx::PlayStation::run()
{
	//disassemble("bios");
	for (uint32_t i = 0; i < m_cpu_tests.size(); i++)
	{
		std::string testHeader = "==== TEST [ " + std::to_string(i + 1) + " / " + 
			                     std::to_string(m_cpu_tests.size()) + "]: " + 
								 m_cpu_tests[i]->getName() + " ====\n";
		m_context->getDebugger()->log(testHeader);
		
		m_bus->loadTest(m_cpu_tests[i], m_cpu_tests[i]->getSize());
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