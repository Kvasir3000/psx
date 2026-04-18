#include "testbuilder.h"
#include "testsuits.h"

namespace testing
{
	TestBuilder::TestBuilder()
	{

	};

	TestBuilder TestBuilder::addTestStore()
	{
		TestStore test;
		m_tests.emplace_back((Test)test);
		return *this;
	}

	TestBuilder TestBuilder::addTestMove()
	{
		TestMove test;
		m_tests.emplace_back((Test)test);
		return *this;
	}

	TestBuilder TestBuilder::addTestSetOn()
	{
		TestSetOn test;
		m_tests.emplace_back((Test)test);
		return *this;
	}

	TestBuilder TestBuilder::addTestArithmeticOps()
	{
		TestArithmeticOps test;
		m_tests.emplace_back((Test)test);
		return *this;
	}

	TestBuilder  TestBuilder::addTestLoadWLR()
	{
		TestLoadWLR test;
		m_tests.emplace_back((Test)test);
		return *this;
	}

	TestBuilder TestBuilder::addTestLoadOperations()
	{
		TestLoadOperations test;
		m_tests.emplace_back((Test)test);
		return *this;
	}

	const std::vector<Test>& TestBuilder::build()
	{
		return m_tests;
	}
}