#pragma once
#include <memory>

#include "../inc/bus.h"
#include "../inc/debugger.h"


namespace psx
{
	class Context
	{
	private:
		std::shared_ptr<BUS>      m_bus;
		bool                      m_isDebugMode;
		std::shared_ptr<Debugger> m_debugger;
		
		 
	public:
		Context(std::shared_ptr<BUS> bus);
		std::shared_ptr<BUS> getBus();
		bool isDebug();
		std::shared_ptr<Debugger> getDebugger();
	};
}