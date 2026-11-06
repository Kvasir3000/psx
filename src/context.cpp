#include "../inc/context.h"

psx::Context::Context(std::shared_ptr<psx::BUS> bus, std::shared_ptr<psx::Debugger> debugger) :
	m_bus(bus),
	m_debugger(debugger),
	m_isDebugMode(true)
{

}

std::shared_ptr<psx::BUS> psx::Context::getBus()
{
	return m_bus;
}

bool psx::Context::isDebug()
{
	return m_isDebugMode;
}

std::shared_ptr<psx::Debugger> psx::Context::getDebugger()
{
	return m_debugger;
}