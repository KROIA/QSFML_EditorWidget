#pragma once

#include "UnitTest.h"
#include "QSFML_EditorWidget.h"
#include <QApplication>
#include <QObject>
#include <QCoreapplication>
#include <QTimer>

#include "Factories/Factories.h"


class TST_ManyObjects : public QObject, public UnitTest::Test
{
	TEST_CLASS(TST_ManyObjects);
	Q_OBJECT;
public:
	TST_ManyObjects()
		: Test("TST_ManyObjects")
	{
		ADD_TEST(TST_ManyObjects::test);


		setBreakOnFail(false);

		connect(&m_stopTimer, &QTimer::timeout, this, &TST_ManyObjects::onTimeout);
		connect(&m_update, &QTimer::timeout, this, &TST_ManyObjects::onUpdate);
	}

private slots:
	void onTimeout()
	{
		m_update.stop();
		qApp->quit();
	}
	void onUpdate()
	{
		if(!m_currentCanvas)
			return;


		m_stats.push_back(m_currentCanvas->getLastStats());
	}
private:
	// Vars
	QTimer m_stopTimer;
	QTimer m_update;

	QSFML::Canvas *m_currentCanvas;
	std::vector<QSFML::Utilities::Stats> m_stats;

	// Functions
	void onFail(const std::string& message) override
	{
		//std::cout << "Failed: " << message << std::endl;
	}
	void onTestsStart() override
	{ 
	
	}
	void onTestsEnd() override
	{ 
	
	}
	bool processStats(TestResults& results)
	{
		TEST_START(results);
		TEST_MESSAGE("Stats: ");

		double averageFPS = 0;
		double averageFrameTime = 0;
		double averageEventTime = 0;
		double averageUpdateTime = 0;
		double averageDrawTime = 0;

		for (const auto& stat : m_stats)
		{
			averageFPS += stat.getFPS();
			averageFrameTime += stat.getFrameTime();
			averageEventTime += stat.getEventTime();
			averageUpdateTime += stat.getUpdateTime();
			averageDrawTime += stat.getDrawTime();
		}

		averageFPS /= m_stats.size();
		averageFrameTime /= m_stats.size();
		averageEventTime /= m_stats.size();
		averageUpdateTime /= m_stats.size();
		averageDrawTime /= m_stats.size();

		TEST_MESSAGE("Average FPS: " + std::to_string(averageFPS));
		TEST_MESSAGE("Average Frame Time: " + std::to_string(averageFrameTime) + " ms");
		TEST_MESSAGE("Average Event Time: " + std::to_string(averageEventTime) + " ms");
		TEST_MESSAGE("Average Update Time: " + std::to_string(averageUpdateTime) + " ms");
		TEST_MESSAGE("Average Draw Time: " + std::to_string(averageDrawTime) + " ms");



		TEST_END;
	}

	// Tests
	bool test(TestResults& results)
	{
		TEST_START(results);
		size_t objectCount = 1000;
		size_t verteciesCount = 10;

		
		QSFML::CanvasSettings settings;
		settings.timing.frameTime = 0;
		QSFML::Canvas canvas(nullptr, settings);
		canvas.show();
		canvas.start();
		m_stopTimer.start(5000);
		m_update.start(10);
		m_currentCanvas = &canvas;
		m_stats.clear();
		m_stats.reserve(m_stopTimer.interval()/ m_update.interval() + 50);

		sf::Color color1 = sf::Color::Red;
		sf::Color color2 = sf::Color::Green;

		canvas.addObject(new QSFML::Objects::DefaultEditor());
		
		for (size_t i = 0; i < objectCount; ++i)
		{
			sf::Vector2f randPos = QSFML::Utilities::RandomEngine::getVector() * 1000.f;
			sf::Color color = QSFML::Color::lerpLinear(color1, color2, (float)i / objectCount);
			canvas.addObject(Factories::randomShapeObject(randPos, 10, color, verteciesCount));
		}



		qApp->exec();
		m_currentCanvas = nullptr;
		processStats(results);
		TEST_END;
	}



};
