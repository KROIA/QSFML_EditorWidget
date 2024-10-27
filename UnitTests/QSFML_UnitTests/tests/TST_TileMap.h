#pragma once

#include "UnitTest.h"
#include "QSFML_EditorWidget.h"
#include <QObject>
#include <QCoreapplication>
#include <QDir>
#include <QTimer>

//#define ENABLE_SCREEN_CAPTURE

using namespace QSFML;
using namespace QSFML::Utilities;

class MyChunk : public Utilities::Chunk
{
	public:
	MyChunk(const sf::Vector2i& pos, const Resources& resources, float scale)
		: Chunk(pos, resources, scale)
	{
	}

	private:
	void onGenerate() override
	{
		m_chunkData = new ChunkData();
		const sf::Vector2i m_position = getPosition();

		//static Utilities::PerlinNoise noise;
		static FastNoiseLite noise1;
		noise1.SetNoiseType(FastNoiseLite::NoiseType_Perlin);
		noise1.SetFrequency(0.005);
		noise1.SetFractalOctaves(10);
		noise1.SetFractalGain(1);
		noise1.SetFractalWeightedStrength(0.5);
		noise1.SetCellularDistanceFunction(FastNoiseLite::CellularDistanceFunction_EuclideanSq);
		noise1.SetCellularReturnType(FastNoiseLite::CellularReturnType_Distance);
		noise1.SetFractalType(FastNoiseLite::FractalType::FractalType_FBm);
		noise1.SetSeed(1234);

		static FastNoiseLite noise2;
		noise2.SetNoiseType(FastNoiseLite::NoiseType_Perlin);
		noise2.SetFractalOctaves(1);
		noise2.SetFractalWeightedStrength(0.0);
		noise2.SetFrequency(0.005);
		noise2.SetFractalType(FastNoiseLite::FractalType::FractalType_Ridged);
		//noise.SetCellularJitter(5.0);


		float scale = 1;

		/*struct ColorF
		{
			float r = 0 , g=0, b=0, a=0;
		};
		ColorF color[CHUNK_LOW_RES_SIZE * CHUNK_LOW_RES_SIZE];*/

		enum MapType
		{
			Grass,
			Dirt,
			Water,
			Sand,
			Stone
		};
		for (size_t x = 0; x < CHUNK_SIZE; x++)
		{
			for (size_t y = 0; y < CHUNK_SIZE; y++)
			{
				sf::Vector2f rawPos((float)x + m_position.x, (float)y + m_position.y);
				sf::Vector2f pos = rawPos * scale;
				float noise2Val = noise2.GetNoise((float)rawPos.x, (float)rawPos.y);
				float noise1Val = noise1.GetNoise(pos.x, pos.y) + 0.1;
				float value = noise1Val * 10;

				float riverNoise = (0.92 - noise2Val) * 2;
				//riverNoise = 10;
				//value = value * value;


				size_t textureIndex = MapType::Grass;


				float riverRange = 0.2;
				if (riverNoise < riverRange)
				{
					if (riverNoise > 0)
					{
						textureIndex = MapType::Sand;
					}
					else
					{
						textureIndex = MapType::Water;
					}
				}
				else
				{

					if (value < 0.1f)
						textureIndex = MapType::Water;
					else if (value < 0.4f)
						textureIndex = MapType::Sand;
					else if (value < 0.6f)
						textureIndex = MapType::Dirt;
					else if (value < 0.7f)
						textureIndex = MapType::Stone;

				}






				m_chunkData->setTextureIndex(x, y, textureIndex);
			}
		}
	}
};

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
		scene->applyObjectChanges();

#ifdef ENABLE_SCREEN_CAPTURE
		Utilities::CameraRecorder* recorder = new Utilities::CameraRecorder(scene->getDefaultCamera(), 4);
		QTimer* singleShotTimer = new QTimer(this);
		singleShotTimer->singleShot(1000, [recorder]()
									{
										recorder->startCapture(500, 0.03, "screenshots/TileMap");
									});
		singleShotTimer->start();
#endif

		

		Utilities::ChunkManager *chunkManager = new Utilities::ChunkManager(Assets::TextureManager::getTexture("MapTiles0.png"), sf::Vector2u(5, 1));
		chunkManager->setChunkFactory(Chunk::createFactory<MyChunk>());
		//chunkManager->loadChunk(sf::Vector2f(-256, 16));
		chunkManager->loadChunk(sf::FloatRect(-1000,-1000,2000,2000),32,true);
		
		gameObj->addUpdateFunction([&chunkManager](const Objects::GameObject& obj)
			{
				//sf::Vector2f pos = obj.getMouseWorldPosition();

				//chunkManager.loadChunk(pos);

				//return;
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::E))
				{
					static QSFML::Objects::GameObjectPtr camController = obj.getSceneParent()->getFirstObjectRecursive<Objects::CameraController>();
					if(camController)
						camController->rotate(1);
				}else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Q))
				{
					static QSFML::Objects::GameObjectPtr camController = obj.getSceneParent()->getFirstObjectRecursive<Objects::CameraController>();
					if (camController)
						camController->rotate(-1);
				}
					

				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space))
				{
					for (int i = 0; i < 100; ++i)
					{
						static int edgeIndex = 0;
						static int edgeSize = 0;
						static int edgeCounter = 0;

						static const int chunkSize = chunkManager->getChunkSpacing();
						// load chunks in a spiral pattern

						static sf::Vector2f pos = sf::Vector2f(5, 5);
						chunkManager->loadChunk(pos);


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
				chunkManager->draw(target, states);
			});

		qApp->exec();
		delete chunkManager;
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
		editor->getCamera()->setMaxZoom(100);
		editor->getCamera()->setMaxMovingBounds(sf::FloatRect(-10000, -10000, 20000, 20000));
		//editor->getCamera()->setRotation(5);

		return scene;
	}

};
