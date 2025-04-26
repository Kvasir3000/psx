#include "../inc/context.h"

psx::Context::Context(std::shared_ptr<psx::BUS> bus) :
	m_bus(bus),
	m_debugger(std::make_shared<Debugger>()),
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