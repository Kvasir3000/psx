#pragma once
#include <vector>
#include <memory>
#include "test.h"

namespace testing
{

	class TestBuilder 
	{
		std::vector<Test> m_tests;
	public:
		TestBuilder();

		TestBuilder addTestStore();
		TestBuilder addTestMove();
		TestBuilder addTestSetOn();
		TestBuilder addTestArithmeticOps();
		TestBuilder addTestLoadWLR();
		TestBuilder addTestLoadOperations();
		const std::vector<Test>& build();
	};
}