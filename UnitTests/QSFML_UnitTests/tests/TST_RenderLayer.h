#pragma once

#include "UnitTest.h"
#include "QSFML_EditorWidget.h"
#include <QObject>
#include <QCoreapplication>
#include <QTimer>

 
using namespace QSFML;

class TST_RenderLayer : public QObject, public UnitTest::Test
{
	TEST_CLASS(TST_RenderLayer);
	Q_OBJECT
public:
	TST_RenderLayer()
		: Test("TST_RenderLayer")
	{
		ADD_TEST(TST_RenderLayer::test1);
		ADD_TEST(TST_RenderLayer::test2);

		connect(&m_stopTimer, &QTimer::timeout, this, &TST_RenderLayer::onTimeout);
		m_stopTimer.setInterval(60000);
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
		Scene* scene = createDefaultScene();

		Objects::GameObjectPtr obj = new Objects::GameObject("RootObject");
		Objects::GameObjectPtr child = new Objects::GameObject("ChildObject");
		
		Components::PointPainter* painter1 = new Components::PointPainter("Painter1");
		Components::PointPainter* painter2 = new Components::PointPainter("Painter2");

		painter1->addPoint({ 0,0 });
		painter1->setRadius(10);
		painter1->setColor(sf::Color::Red);

		painter2->addPoint({ 5,0 });
		painter2->setRadius(10);
		painter2->setColor(sf::Color::Green);

		obj->setPosition({ 100,100 });
		obj->addComponent(painter1);
		child->addComponent(painter2);

		obj->addChild(child);
		scene->addObject(obj);

		scene->applyObjectChanges();

		obj->setRenderLayer(RenderLayer::layer_2);
		child->setRenderLayer(RenderLayer::layer_1);

		//obj->setDrawOrder({
		//	Objects::GameObject::DrawSequenceElement::childs,
		//	Objects::GameObject::DrawSequenceElement::components
		//				  });

		qApp->exec();
		delete scene;
		m_stopTimer.stop();
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


	Scene* createDefaultScene()
	{
		QSFML::SceneSettings settings;
		settings.timing.frameTime = 0;
		QSFML::Scene* scene = new QSFML::Scene(nullptr, settings);
		scene->start();
		m_stopTimer.start();

		scene->addObject(new QSFML::Objects::DefaultEditor());
		return scene;
	}

};
