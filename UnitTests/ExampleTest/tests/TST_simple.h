#pragma once

#include "UnitTest.h"
#include <QObject>
#include <QCoreapplication>


#define STRINGIFY(x) 


class TST_simple : public UnitTest::Test
{
	TEST_CLASS(TST_simple)
public:
	TST_simple()
		: Test("TST_simple")
	{
		ADD_TEST(TST_simple::test1);
		ADD_TEST(TST_simple::test2);

	}

private:

	// Tests
	bool test1(TestResults& results)
	{
		TEST_START(results);
		//TEST_MESSAGE(std::to_string(__LINE__));
		int a = 0;
		TEST_MESSAGE("is a == 0?");
		TEST_ASSERT(a == 0);

		TEST_END;
	}




	bool test2(TestResults& results)
	{
		TEST_START(results);

		int a = 0;
		TEST_ASSERT_M(a == 0, "is a == 0?");

		int b = 0;
		if (b != 0)
		{
			TEST_FAIL("b is not 0");
		}

		// fails if a != b
		TEST_COMPARE(a, b);

		TEST_END;
	}

};
