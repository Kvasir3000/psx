#include "testbuilder.h"
#include "testsuits.h"

namespace testing
{
	TestBuilder::TestBuilder()
	{

	};

	TestBuilder& TestBuilder::addTestStore()
	{
		return addTest<TestStore>();
	}

	TestBuilder& TestBuilder::addTestMove()
	{
		return addTest<TestMove>();
	}

	TestBuilder& TestBuilder::addTestSetOn()
	{
		return addTest<TestSetOn>();
	}

	TestBuilder& TestBuilder::addTestArithmeticOps()
	{
		return addTest<TestArithmeticOps>();
	}

	TestBuilder& TestBuilder::addTestLoad()
	{
		return addTest<TestLoad>();
	}

	TestBuilder& TestBuilder::addTestLoadWLR()
	{
		return addTest<TestLoadWLR>();
	}

	TestBuilder& TestBuilder::addTestLoadByteOperations()
	{
		return addTest<TestLoadByteOperations>();
	}

	TestBuilder& TestBuilder::addTestLoadHalfwordOperations()
	{
		return addTest<TestLoadHalfwordOperations>();
	}

	TestBuilder& TestBuilder::addTestLoadCOP2()
	{
		return addTest<TestLoadCOP2>();
	}

	TestBuilder& TestBuilder::addTestBranch()
	{
		return addTest<TestBranch>();
	}	
	
	TestBuilder& TestBuilder::addTestJump()
	{
		return addTest<TestJump>();
	}

	const std::vector<std::shared_ptr<Test>>& TestBuilder::build() const
	{
		for (auto test : m_tests)
		{
			test.get()->link();
		}
		return m_tests;
	}
}