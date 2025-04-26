#pragma once
#include <memory>

#include "../inc/cpu.h"
#include "../inc/bus.h"
#include "../inc/context.h"

namespace psx
{
	class PlayStation
	{
	private: 
		std::shared_ptr<psx::BUS>     m_bus;
		std::shared_ptr<Context>      m_context;
		mips::CPU                     m_cpu;
		bool                          m_isRunning = false; 

	public: 
		PlayStation();
		~PlayStation();

		void readProgramFile();
		
		void run();
	};
}