#pragma once

#include "UnitTest.h"
#include "QSFML_EditorWidget.h"
#include <QObject>
#include <QCoreapplication>
#include <QDir>
#include <QTimer>


using namespace QSFML;

class TST_TextureManager: public QObject, public UnitTest::Test
{
	TEST_CLASS(TST_TextureManager);
	Q_OBJECT;
public:
	TST_TextureManager()
		: Test("TST_TextureManager")
	{
		ADD_TEST(TST_TextureManager::load1);
		ADD_TEST(TST_TextureManager::load2);
		ADD_TEST(TST_TextureManager::filteredLoad);
		//ADD_TEST(TST_TextureManager::displayTest);

		connect(&m_stopTimer, &QTimer::timeout, this, &TST_TextureManager::onTimeout);

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
	TEST_FUNCTION(load1)
	{
		TEST_START;
		Assets::TextureManager::clear();
		TEST_ASSERT(Assets::TextureManager::isLoaded() == false);
		TEST_ASSERT(Assets::TextureManager::getTextureCount() == 0);


		// Get the amount of images in the asset folder
		QDir dir(QString::fromStdString(assetPath));
		dir.setNameFilters(Assets::TextureManager::getFileNameFilters());
		dir.setFilter(QDir::Files | QDir::NoDotAndDotDot | QDir::NoSymLinks);
		QFileInfoList list = dir.entryInfoList();
		size_t imageCount = list.size();

		TEST_MESSAGE("Loading assets from: " + assetPath);
		TEST_ASSERT(Assets::TextureManager::loadTextures(assetPath));

		TEST_ASSERT(Assets::TextureManager::isLoaded());
		TEST_ASSERT(Assets::TextureManager::getTextureCount() == imageCount);
	}

	TEST_FUNCTION(load2)
	{
		TEST_START;
		Assets::TextureManager::clear();
		TEST_ASSERT(Assets::TextureManager::isLoaded() == false);
		TEST_ASSERT(Assets::TextureManager::getTextureCount() == 0);

		Assets::TextureManager::setFileNameFilters({ "*.png", "*.jpg", "*.jpeg", "*.bmp" });

		std::string invalidPath = assetPath + "/invalid";
		TEST_MESSAGE("Loading assets from invalid path: " + invalidPath);

		// Should return false and print an error message
		TEST_ASSERT(Assets::TextureManager::loadTextures(invalidPath) == false);

		TEST_ASSERT(Assets::TextureManager::isLoaded() == false);
		TEST_ASSERT(Assets::TextureManager::getTextureCount() == 0);

		// Should print an error message
		TEST_ASSERT(Assets::TextureManager::getTexture("XX").getSize().x == 0);

		// Should trigger a break point in dbg mode, in release it will crash
		TEST_ASSERT(Assets::TextureManager::getTexture(0).getSize().x == 0);

	}




	TEST_FUNCTION(filteredLoad)
	{
		TEST_START;
		Assets::TextureManager::clear();
		TEST_ASSERT(Assets::TextureManager::isLoaded() == false);
		TEST_ASSERT(Assets::TextureManager::getTextureCount() == 0);

		QStringList  filters = { "*.png" };

		Assets::TextureManager::setFileNameFilters(filters);
		TEST_ASSERT(Assets::TextureManager::getFileNameFilters() == filters);

		// Get the amount of images in the asset folder
		QDir dir(QString::fromStdString(assetPath));
		dir.setNameFilters(filters);
		dir.setFilter(QDir::Files | QDir::NoDotAndDotDot | QDir::NoSymLinks);
		QFileInfoList list = dir.entryInfoList();
		size_t imageCount = list.size();

		TEST_MESSAGE("Loading assets from: " + assetPath);
		TEST_ASSERT(Assets::TextureManager::loadTextures(assetPath));

		TEST_ASSERT(Assets::TextureManager::isLoaded());
		TEST_ASSERT(Assets::TextureManager::getTextureCount() == imageCount);
	}

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
		Scene::setProfilerOutputFileName("TST_TextureManager_displayTest.prof");
		Scene* scene = createDefaultScene();
		Objects::GameObjectPtr gameObj = new Objects::GameObject();
		scene->addObject(gameObj);

		gameObj->addDrawFunction([nextTextureTime](const Objects::GameObject& obj, sf::RenderTarget& target, sf::RenderStates states)
			{
				float time = obj.getAge()*10;
				size_t textureIndex = ((int)time / ((int)nextTextureTime+1)) % Assets::TextureManager::getTextureCount();

				const sf::Texture& texture = Assets::TextureManager::getTexture(textureIndex);
				sf::Sprite sprite(texture);

				sf::RectangleShape shape(sf::Vector2f(100, 100));
				shape.setTexture(&texture);

				target.draw(sprite);
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
		m_stopTimer.start();

		scene->addObject(new Objects::DefaultEditor());
		return scene;
	}

};
