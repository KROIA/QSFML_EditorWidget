#pragma once

#include "UnitTest.h"
#include "QSFML_EditorWidget.h"
#include <QObject>
#include <QCoreapplication>
#include <QTimer>


class TST_Pathfinder : public QObject, public UnitTest::Test
{
	TEST_CLASS(TST_Pathfinder);
	Q_OBJECT;
public:
	TST_Pathfinder()
		: Test("TST_Pathfinder")
	{
		ADD_TEST(TST_Pathfinder::test1);
		ADD_TEST(TST_Pathfinder::test2);


		connect(&m_stopTimer, &QTimer::timeout, this, &TST_Pathfinder::onTimeout);
		m_stopTimer.setInterval(1000);
	}


private slots:
	void onTimeout()
	{
		m_stopTimer.stop();
		qApp->quit();
	}
private:
	QTimer m_stopTimer;

	// Tests
	TEST_FUNCTION(test1)
	{
		TEST_START;

		int a = 0;
		TEST_MESSAGE("is a == 0?");
		TEST_ASSERT(a == 0);
	}




	TEST_FUNCTION(test2)
	{
		TEST_START;

		int a = 0;
		TEST_ASSERT_M(a == 0, "is a == 0?");

		int b = 0;
		if (b != 0)
		{
			TEST_FAIL("b is not 0");
		}

		// fails if a != b
		TEST_COMPARE(a, b);
	}

};
