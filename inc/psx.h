#pragma once
#include <memory>

#include "../inc/cpu/cpu.h"
#include "../inc/bus.h"
#include "../inc/context.h"
#include "../tests/testbuilder.h"

namespace psx
{
	class PlayStation
	{
	public:
		PlayStation();
		~PlayStation();

		void readProgramFile();
		void disassemble(const std::string& name);

		void run();
	private: 
		std::shared_ptr<psx::BUS>                     m_bus;
		std::shared_ptr<psx::Debugger>                m_debugger;
		std::shared_ptr<Context>                      m_context;
		mips::CPU								      m_cpu;
		bool										  m_isRunning = false; 
		std::vector<std::shared_ptr<testing::Test>>   m_cpu_tests;

	};
}