#pragma once

#include "UnitTest.h"
#include "QSFML_EditorWidget.h"
#include <QObject>
#include <QCoreapplication>
#include <QDir>
#include <QTimer>

using namespace QSFML;

class TST_TileMap: public QObject, public UnitTest::Test
{
	TEST_CLASS(TST_TileMap);
	Q_OBJECT;
public:
	TST_TileMap()
		: Test("TST_TileMap")
	{
		ADD_TEST(TST_TileMap::displayTest);

		connect(&m_stopTimer, &QTimer::timeout, this, &TST_TileMap::onTimeout);

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

		// Create a scene
		Scene::setProfilerOutputFileName("TST_TileMap_displayTest.prof");
		Scene* scene = createDefaultScene();
		Objects::GameObjectPtr gameObj = new Objects::GameObject();
		scene->addObject(gameObj);

		Utilities::ChunkManager chunkManager(Assets::TextureManager::getTexture("MapTiles0.png"), sf::Vector2u(5, 1));
		chunkManager.loadChunk(sf::Vector2f(9, 9));
		
		gameObj->addUpdateFunction([&chunkManager](const Objects::GameObject& obj)
			{
				//sf::Vector2f pos = obj.getMouseWorldPosition();

				//chunkManager.loadChunk(pos);

				//return;
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space))
				{
					for (int i = 0; i < 100; ++i)
					{
						static int edgeIndex = 0;
						static int edgeSize = 0;
						static int edgeCounter = 0;

						static const int chunkSize = chunkManager.getChunkSpacing();
						// load chunks in a spiral pattern

						static sf::Vector2f pos = sf::Vector2f(0, 0);
						chunkManager.loadChunk(pos);


						switch (edgeIndex)
						{
							case 0:
								pos.x += chunkSize;
								break;
							case 1:
								pos.y += chunkSize;
								break;
							case 2:
								pos.x -= chunkSize;
								break;
							case 3:
								pos.y -= chunkSize;
								break;
						}



						edgeCounter++;
						if (edgeCounter >= edgeSize)
						{
							edgeCounter = 0;
							edgeIndex++;
							if (edgeIndex >= 4)
							{
								edgeIndex = 0;
								edgeSize += 2;
								pos.x -= chunkSize;
								pos.y -= chunkSize;
							}
						}
					}

				}
			});

		gameObj->addDrawFunction([&chunkManager](const Objects::GameObject& obj, sf::RenderTarget& target, sf::RenderStates states)
			{
				chunkManager.draw(target, states);
			});

		qApp->exec();
		m_stopTimer.stop();
		delete scene;
	}




	Scene* createDefaultScene()
	{
		SceneSettings settings;
		settings.timing.frameTime = 0.02;
		settings.contextSettings.antialiasingLevel = 0;
		Scene* scene = new Scene(nullptr, settings);
		//scene.show();
		scene->start();
		//m_stopTimer.start();

		Objects::DefaultEditor* editor = new Objects::DefaultEditor("Editor", sf::Vector2f(1000, 800));
		scene->addObject(editor);
		editor->getCamera()->setMinZoom(0.01);

		return scene;
	}

};
