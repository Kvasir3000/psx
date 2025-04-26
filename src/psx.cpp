#include "../inc/psx.h"

#include <iostream>
#include <fstream>

psx::PlayStation::PlayStation() :
	m_bus(std::make_shared<BUS>()),
	m_context(std::make_shared<Context>(m_bus)),
	m_cpu(mips::CPU(m_context)),
	m_isRunning(true)
{	
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
	while (m_isRunning)
	{
		m_isRunning = m_cpu.emuCycle();
	}
}