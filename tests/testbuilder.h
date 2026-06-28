#pragma once
#include <vector>
#include <memory>
#include "test.h"

namespace testing
{

	class TestBuilder 
	{
		std::vector<std::shared_ptr<Test>> m_tests;
		template<typename TestType>
		TestBuilder& addTest()
		{
			std::shared_ptr<TestType> test = std::make_shared<TestType>();
			m_tests.emplace_back(std::move(test));
			return *this;
		}

	public:
		TestBuilder();
		
		TestBuilder& addTestStore();
		TestBuilder& addTestMove();
		TestBuilder& addTestSetOn();
		TestBuilder& addTestArithmeticOps();
		TestBuilder& addTestLoadWLR();
		TestBuilder& addTestLoadOperations();
		TestBuilder& addTestLoadCOP2();
		TestBuilder& addTestBranch();
		TestBuilder& addTestJump();
		const std::vector<std::shared_ptr<Test>>& build() const;
	};
}