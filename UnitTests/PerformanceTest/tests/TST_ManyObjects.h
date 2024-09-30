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
		, m_tree(nullptr, QSFML::Utilities::AABB({ 0,0 }, { 800,600 }), 4)
	{
		ADD_TEST(TST_ManyObjects::drawTest);
		ADD_TEST(TST_ManyObjects::collisionTest);


		setBreakOnFail(false);

		connect(&m_stopTimer, &QTimer::timeout, this, &TST_ManyObjects::onTimeout);
		
	}

private slots:
	void onTimeout()
	{
		m_update.stop();
		qApp->quit();

		m_averageCollisionCheckTimeFull/= m_stats.size();
		m_averageCollisionCheckTime/= m_stats.size();
	}
	void onDrawTest_Update()
	{
		if(!m_currentScene)
			return;


		m_stats.push_back(m_currentScene->getLastStats());
	}
	void onCollisionTest_Update()
	{
		if (!m_currentScene)
			return;

		m_stats.push_back(m_currentScene->getLastStats());

		TimePoint t1 = std::chrono::high_resolution_clock::now();
		m_tree.clear();
		m_tree.shrink();
		m_tree.insert(m_currentScene->getObjects());

		std::vector<QSFML::Utilities::Collisioninfo> collisions;
		std::vector<sf::Vector2f> collisionPoints;

		TimePoint t11 = std::chrono::high_resolution_clock::now();
		m_tree.checkCollisions(collisions, false);
		TimePoint t22 = std::chrono::high_resolution_clock::now();
		auto millis2 = std::chrono::duration_cast<std::chrono::milliseconds>(t22 - t11).count();

		collisionPoints.reserve(collisions.size());
		for (auto& el : collisions)
			collisionPoints.push_back(el.collisionPos);

		TimePoint t2 = std::chrono::high_resolution_clock::now();
		auto millis1 = std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count();

		if(m_pointPainter)
			m_pointPainter->setPoints(collisionPoints);

		m_averageCollisionCheckTimeFull += millis1;
		m_averageCollisionCheckTime += millis2;
	}
private:
	// Vars
	QTimer m_stopTimer;
	QTimer m_update;

	QSFML::Scene *m_currentScene;
	//QSFML::Objects::GameObject* m_pointPainterObj = nullptr;
	QSFML::Components::PointPainter* m_pointPainter = nullptr;
	TestResults *m_currentResults;
	std::vector<QSFML::Utilities::Stats> m_stats;
	QSFML::Utilities::ObjectQuadTree m_tree;

	double m_averageCollisionCheckTimeFull = 0;
	double m_averageCollisionCheckTime = 0;


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
	TEST_FUNCTION(processStats)
	{
		TEST_START(results);
		TEST_MESSAGE("Stats:");

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
		TEST_MESSAGE("Average CollisionCheckFull Time: " + std::to_string(m_averageCollisionCheckTimeFull) + " ms");
		TEST_MESSAGE("Average CollisionCheck Time: " + std::to_string(m_averageCollisionCheckTime) + " ms");
	}

	// Tests
	TEST_FUNCTION(drawTest)
	{
		TEST_START(results);
		size_t objectCount = 10000;
		size_t verteciesCount = 5;

		
		QSFML::SceneSettings settings;
		settings.timing.frameTime = 0;
		QSFML::Scene Scene(nullptr, settings);
		Scene.show();
		Scene.start();
		QSFML::Scene::setProfilerOutputFileName("drawTest.prof");
		m_stopTimer.start(1000);
		m_update.start(10);
		connect(&m_update, &QTimer::timeout, this, &TST_ManyObjects::onDrawTest_Update);
		m_currentScene = &Scene;
		m_currentResults = &results;
		m_stats.clear();
		m_stats.reserve(m_stopTimer.interval()/ m_update.interval() + 50);

		sf::Color color1 = sf::Color::Red;
		sf::Color color2 = sf::Color::Green;

		Scene.addObject(new QSFML::Objects::DefaultEditor());
		
		for (size_t i = 0; i < objectCount; ++i)
		{
			sf::Vector2f randPos = QSFML::Utilities::RandomEngine::getVector() * 1000.f;
			sf::Color color = QSFML::Color::lerpLinear(color1, color2, (float)i / objectCount);
			Scene.addObject(Factories::randomShapeObject(randPos, 5, color, verteciesCount));
		}



		qApp->exec();
		m_currentScene = nullptr;
		m_currentResults = nullptr;
		processStats(results);
	}

	TEST_FUNCTION(collisionTest)
	{
		TEST_START(results);
		size_t objectCount = 10000;
		size_t verteciesCount = 5;


		QSFML::SceneSettings settings;
		settings.timing.frameTime = 0;
		QSFML::Scene Scene(nullptr, settings);
		Scene.show();
		Scene.start();
		QSFML::Scene::setProfilerOutputFileName("collisionTest.prof");
		m_tree.setStatsManager(&Scene);
		m_stopTimer.start(1000);
		m_update.start(10);
		connect(&m_update, &QTimer::timeout, this, &TST_ManyObjects::onCollisionTest_Update);
		m_currentScene = &Scene;
		m_currentResults = &results;
		m_stats.clear();
		m_stats.reserve(m_stopTimer.interval() / m_update.interval() + 50);

		sf::Color color1 = sf::Color::Red;
		sf::Color color2 = sf::Color::Green;

		Scene.addObject(new QSFML::Objects::DefaultEditor());
		QSFML::Objects::GameObject * pointPainterObj = new QSFML::Objects::GameObject();
		m_pointPainter = new QSFML::Components::PointPainter();
		m_pointPainter->setColor(sf::Color::Yellow);
		pointPainterObj->addComponent(m_pointPainter);
		QSFML::Utilities::ObjectQuadTree::ObjectQuadTreePainter* treePainter = m_tree.createPainter();
		//treePainter->enableText(false);
		pointPainterObj->addComponent(treePainter);

		Scene.addObject(pointPainterObj);


		for (size_t i = 0; i < objectCount; ++i)
		{
			sf::Vector2f randPos = QSFML::Utilities::RandomEngine::getVector({ 0,0 }, { 800,600 });
			sf::Color color = QSFML::Color::lerpLinear(color1, color2, (float)i / objectCount);
			Scene.addObject(Factories::randomShapeObject(randPos, 5, color, verteciesCount));
		}
		


		qApp->exec();
		m_currentScene = nullptr;
		m_currentResults = nullptr;
		processStats(results);
		m_tree.setStatsManager(nullptr);
		m_pointPainter = nullptr;
	}

};
