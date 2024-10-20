#pragma once

#include "UnitTest.h"
#include "QSFML_EditorWidget.h"
#include <QObject>
#include <QCoreapplication>
#include <QDir>
#include <QTimer>

using namespace QSFML;

class TST_TextureAnimation: public QObject, public UnitTest::Test
{
	TEST_CLASS(TST_TextureAnimation);
	Q_OBJECT;
public:
	TST_TextureAnimation()
		: Test("TST_TextureAnimation")
	{
		ADD_TEST(TST_TextureAnimation::displayTest);

		connect(&m_stopTimer, &QTimer::timeout, this, &TST_TextureAnimation::onTimeout);

		m_stopTimer.setInterval(20000);
	}

private slots:
	void onTimeout()
	{
		m_stopTimer.stop();
		qApp->quit();
	}

private:
	QTimer m_stopTimer;
	const std::string assetPath = "../UnitTests/QSFML_UnitTests/assets";
	// Tests
	

	TEST_FUNCTION(displayTest)
	{
		TEST_START;
		Assets::TextureManager::clear();
		TEST_ASSERT(Assets::TextureManager::isLoaded() == false);
		TEST_ASSERT(Assets::TextureManager::getTextureCount() == 0);

		TEST_MESSAGE("Loading assets from: " + assetPath);
		TEST_ASSERT(Assets::TextureManager::loadTextures(assetPath));

		float timerTime = m_stopTimer.interval()/1000;
		float nextTextureTime = timerTime / (Assets::TextureManager::getTextureCount() + 1);

		// Create a scene
		Scene::setProfilerOutputFileName("TST_TextureAnimation_displayTest.prof");
		Scene* scene = createDefaultScene();
		Objects::GameObjectPtr gameObj = new Objects::GameObject();
		scene->addObject(gameObj);

		Components::TextureAnimation* textureAnimation1 = new Components::TextureAnimation(Assets::TextureManager::getTexture("animations/animation-walking.png"), sf::Vector2u(12, 4));
		Components::TextureAnimation* textureAnimation2 = new Components::TextureAnimation(Assets::TextureManager::getTexture("animations/loading-animation.png"), sf::Vector2u(5, 4));
		gameObj->addComponent(textureAnimation1);
		gameObj->addComponent(textureAnimation2);
		//gameObj->updateObjectChanges();

		Components::TextureAnimation::Animation animation1("walk_front",
			{ {0, 0.1f}, {1, 0.1f}, {2, 0.1f}, {3, 0.1f}, {4, 0.1f}, {5, 0.1f}, {6, 0.1f}, {7, 0.1f}, {8, 0.1f}, {9, 0.1f}, {10, 0.1f}, {11, 0.1f} });
		Components::TextureAnimation::Animation animation2("walk_left",
			{ {12, 0.1f}, {13, 0.1f}, {14, 0.1f}, {15, 0.1f}, {16, 0.1f}, {17, 0.1f}, {18, 0.1f}, {19, 0.1f}, {20, 0.1f}, {21, 0.1f}, {22, 0.1f}, {23, 0.1f} });
		Components::TextureAnimation::Animation animation3("walk_right",
			{ {24, 0.1f}, {25, 0.1f}, {26, 0.1f}, {27, 0.1f}, {28, 0.1f}, {29, 0.1f}, {30, 0.1f}, {31, 0.1f}, {32, 0.1f}, {33, 0.1f}, {34, 0.1f}, {35, 0.1f} });
		Components::TextureAnimation::Animation animation4("walk_back",
			{ {36, 0.1f}, {37, 0.1f}, {38, 0.1f}, {39, 0.1f}, {40, 0.1f}, {41, 0.1f}, {42, 0.1f}, {43, 0.1f}, {44, 0.1f}, {45, 0.1f}, {46, 0.1f}, {47, 0.1f} });

		Components::TextureAnimation::Animation animation5("right",
			{ {0, 0.1f}, {1, 0.1f}, {2, 0.1f}, {3, 0.1f}, {4, 0.1f}});
		Components::TextureAnimation::Animation animation6("bottom",
			{ {5, 0.1f}, {6, 0.1f}, {7, 0.1f}, {8, 0.1f}, {9, 0.1f}});
		Components::TextureAnimation::Animation animation7("left",
			{ {10, 0.1f}, {11, 0.1f}, {12, 0.1f}, {13, 0.1f}, {14, 0.1f}});
		Components::TextureAnimation::Animation animation8("top",
			{ {15, 0.1f}, {16, 0.1f}, {17, 0.1f}, {18, 0.1f}, {19, 0.1f}});

		textureAnimation1->addAnimation(std::move(animation1));
		textureAnimation1->addAnimation(std::move(animation2));
		textureAnimation1->addAnimation(std::move(animation3));
		textureAnimation1->addAnimation(std::move(animation4));

		textureAnimation2->addAnimation(std::move(animation5));
		textureAnimation2->addAnimation(std::move(animation6));
		textureAnimation2->addAnimation(std::move(animation7));
		textureAnimation2->addAnimation(std::move(animation8));
		
		textureAnimation1->setRepeating(true);
		textureAnimation2->setRepeating(true);
		//textureAnimation1->start("walk_back");
		//textureAnimation1->start(QSFML::vector<QSFML::string>{"walk_front", "walk_left", "walk_right", "walk_back"});
		
		
		sf::Sprite *sprite1 = new sf::Sprite();
		sf::Sprite *sprite2 = new sf::Sprite();
		sprite2->setPosition(200, 0);

		textureAnimation1->bindTo(sprite1);
		textureAnimation2->bindTo(sprite2);

		textureAnimation1->start(QSFML::vector<QSFML::string>{"walk_front", "walk_back"});
		textureAnimation2->start(QSFML::vector<QSFML::string>{"right", "bottom", "left", "top"});
		
		gameObj->addDrawFunction([nextTextureTime, textureAnimation1, textureAnimation2, sprite1, sprite2](const Objects::GameObject& obj, sf::RenderTarget& target, sf::RenderStates states)
			{
				static float time = 0;
				time += obj.getDeltaT();
				//if (time > 0.1)
				//{
				//	time = 0;
				//	obj.logInfo(std::to_string(textureAnimation1->getProgress()));
				//	//textureAnimation->start();
				//}
				textureAnimation1->setSpeed(std::sin(obj.getAge()*0.5));
				textureAnimation2->setSpeed(std::sin(obj.getAge()*0.5));

				//sf::Sprite sprite1(textureAnimation1->getTexture());
				//sprite1.setTextureRect(textureAnimation1->getUVRect());
				//
				//sf::Sprite sprite2(textureAnimation2->getTexture());
				//sprite2.setTextureRect(textureAnimation2->getUVRect());
				//sprite2.setPosition(200, 0);

				target.draw(*sprite1);
				target.draw(*sprite2);
			});

		qApp->exec();
		m_stopTimer.stop();
		delete scene;
	}




	Scene* createDefaultScene()
	{
		SceneSettings settings;
		settings.timing.frameTime = 0.02;
		Scene* scene = new Scene(nullptr, settings);
		//scene.show();
		scene->start();
		//m_stopTimer.start();

		scene->addObject(new Objects::DefaultEditor());
		return scene;
	}

};
