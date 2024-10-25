#include "utilities/TileMap/ChunkManager.h"
#include <SFML/OpenGL.hpp>

namespace QSFML
{
	namespace Utilities
	{
		Log::LogObject& ChunkManager::getLogger()
		{
			static Log::LogObject logger = Log::LogObject("ChunkManager");
			return logger;
		}
		ChunkManager::ChunkManager(const sf::Texture& texture, const sf::Vector2u& mapDim)
			: m_resources{ Assets::TextureMap(texture, mapDim, 0.1) }
			, m_scale(1)
		{
			QSFMLP_GENERAL_FUNCTION(QSFML_COLOR_STAGE_1);
			setChunkFactory(Chunk::createFactory<Chunk>());
			m_needsDrawUpdate = true;
			// Calculate the low level color
			sf::Image image = texture.copyToImage();
			m_resources.lowLevelReplacementColors.resize(mapDim.x * mapDim.y);
			sf::Vector2u subSize = sf::Vector2u(m_resources.textureMap.getSubImageSize());
			
			for (size_t y = 0; y < mapDim.y; y++)
			{
				for (size_t x = 0; x < mapDim.x; x++)
				{
					float r = 0, g = 0, b = 0, a = 0;
					// calculate the average color of the sub image
					for (size_t i = 0; i < subSize.y; i++)
					{
						for (size_t j = 0; j < subSize.x; j++)
						{
							sf::Color color = image.getPixel(x * subSize.x + j, y * subSize.y + i);
							r += color.r;
							g += color.g;
							b += color.b;
							a += color.a;
						}
					}
					r /= subSize.x * subSize.y;
					g /= subSize.x * subSize.y;
					b /= subSize.x * subSize.y;
					a /= subSize.x * subSize.y;
					m_resources.lowLevelReplacementColors[y * mapDim.x + x] = sf::Color(r, g, b, a);
				}
			}
		}
		ChunkManager::~ChunkManager()
		{
			QSFMLP_GENERAL_FUNCTION(QSFML_COLOR_STAGE_1);
			for (size_t i = 0; i < m_asyncChunkLoaderData.size(); ++i)
			{
				m_asyncChunkLoaderData[i]->stop = true;
				for (auto& thread : m_asyncChunkLoaderData[i]->threads)
				{
					thread->join();
					delete thread;
				}
			}

			// Create threads to delete all chunks
			//size_t numThreads = std::thread::hardware_concurrency();
			//
			//if (amountOfChunks < numThreads)
			//	numThreads = 0;
			//numThreads = 0;
			/*std::vector<Chunk*> chunks;
			size_t amountOfChunks = m_loadedChunks.size();
			chunks.resize(amountOfChunks);
			auto it = m_loadedChunks.begin();
			for (size_t i = 0; i < amountOfChunks; ++i)
			{
				chunks[i] = it->second;
				++it;
			}*/
			/*if (numThreads > 1)
			{
				QSFMLP_GENERAL_BLOCK("Delete chunks threaded", QSFML_COLOR_STAGE_2);
				size_t chunksPerThread = amountOfChunks / numThreads;

				std::vector<std::thread> threads;
				
				threads.reserve(numThreads);
				for (size_t i = 0; i < numThreads; ++i)
				{
					size_t startIdx = i * chunksPerThread;
					size_t endIdx = (i + 1) * chunksPerThread;
					if (i == numThreads - 1)
					{
						endIdx = amountOfChunks;
					}
					threads.emplace_back([this, startIdx, endIdx, &chunks]()
						{
							QSFML_PROFILING_THREAD("Delete chunks");
							for (size_t j = startIdx; j < endIdx; ++j)
							{
								//QSFMLP_GENERAL_BLOCK("Delete chunks threaded", QSFML_COLOR_STAGE_3);
								delete chunks[j];
							}
						});
				}
				QSFMLP_GENERAL_BLOCK("Waiting for threads to finish", QSFML_COLOR_STAGE_3);
				for (size_t i = 0; i < numThreads; ++i)
				{
					threads[i].join();
				}
				QSFMLP_GENERAL_END_BLOCK;

			}
			else
			{*/
				QSFMLP_GENERAL_BLOCK("Delete chunks", QSFML_COLOR_STAGE_2);
				// Delete all chunks
				//for (auto& chunk : chunks)
				//{
				//	delete chunk;
				//}
				size_t s = m_loadedChunksVec.size();
				for (size_t i = 0; i < s; ++i)
				{
					delete m_loadedChunksVec[i];
				}
			//}
		}

		void ChunkManager::loadChunk(const sf::FloatRect& area, size_t numThreads, bool async)
		{
			QSFMLP_GENERAL_FUNCTION(QSFML_COLOR_STAGE_1);
			sf::Vector2f realPosStart = sf::Vector2f(area.left / m_scale, area.top / m_scale);
			sf::Vector2f realPosEnd = sf::Vector2f((area.left + area.width) / m_scale, (area.top + area.height) / m_scale);

			sf::Vector2i start = getChunkPosition(realPosStart);
			sf::Vector2i end = getChunkPosition(realPosEnd);

			if (numThreads <= 1)
			{
				for (int x = start.x; x <= end.x; x += Chunk::CHUNK_SIZE)
				{
					for (int y = start.y; y <= end.y; y += Chunk::CHUNK_SIZE)
					{
						loadChunk(sf::Vector2f(x, y));
					}
				}
			}
			else
			{
				if (numThreads > std::thread::hardware_concurrency())
					numThreads = std::thread::hardware_concurrency();

				size_t amountOfChunks = (end.x - start.x) / Chunk::CHUNK_SIZE  *(end.y - start.y)/ Chunk::CHUNK_SIZE;
				numThreads = std::min(numThreads, amountOfChunks);
				size_t chunksPerThread = amountOfChunks / numThreads;

				AsyncChunkLoaderData* chunkLoader = new AsyncChunkLoaderData();
				if(async)
					m_asyncChunkLoaderData.push_back(chunkLoader);

				chunkLoader->threads.reserve(numThreads);
				chunkLoader->loadedChunks.reserve(amountOfChunks);
				chunkLoader->stop = false;
				m_threadsLoading += numThreads;
				for (size_t i = 0; i < numThreads; ++i)
				{
					chunkLoader->threads.emplace_back(new std::thread([this, i, start, end, chunksPerThread, chunkLoader, numThreads, amountOfChunks, async]()
				     {
				    	 size_t startIdx = i * chunksPerThread;
				    	 size_t endIdx = (i + 1) * chunksPerThread;
				    	 if (i == numThreads - 1)
				    	 {
				    		 endIdx = amountOfChunks;
				    	 }
						 std::vector<Chunk*> chunks;
						 chunks.reserve(endIdx - startIdx);

						 sf::IntRect bounds(start.x, start.y, end.x - start.x, end.y - start.y);
						 bool allNotLoaded = true;
						 if (m_generatedChunkBounds.intersects(bounds))
							 allNotLoaded = false;


				    	 for (size_t j = startIdx; j < endIdx; ++j)
				    	 {
							 if (chunkLoader->stop)
								 return;
				    		 int x = start.x + (j % ((end.x - start.x) / Chunk::CHUNK_SIZE)) * Chunk::CHUNK_SIZE;
				    		 int y = start.y + (j / ((end.x - start.x) / Chunk::CHUNK_SIZE)) * Chunk::CHUNK_SIZE;
				    
				    		 sf::Vector2i chunkPos = getChunkPosition(sf::Vector2f(x,y));
				    
							 if(!allNotLoaded)
							 {
								 std::unique_lock<std::mutex> lock(m_mutex);
								 auto it = m_loadedChunks.find(chunkPos); // My be not thread safe
								 if (it != m_loadedChunks.end())
								 {
					
									 getLogger().logInfo("loadChunk_threaded(th=" + std::to_string(i) + "): Pos: {" + std::to_string(chunkPos.x) + "," + std::to_string(chunkPos.y) + "} "
										 "Chunk already loaded");
									 continue;
								 }
							 }
				    		    
				    		 Chunk* chunk = m_chunkFactory->create(chunkPos, m_resources, m_scale);
							 chunks.push_back(chunk);
				    		 //Chunk* chunk = new Chunk(chunkPos, m_resources, m_scale);
				    		 chunk->generate();
				    		 chunk->updateTextureCoords();
							 
							 if (chunks.size() >= 20 && async)
				    		 {
							  {
							 	 std::unique_lock<std::mutex> lock(m_mutex);
							 
							 	 updateGeneratedChunkBounds(chunks);
							 	 for (size_t ch = 0; ch < chunks.size(); ++ch)
							 		 insertNewChunk(chunks[ch]);
							  }
							  chunks.clear();
							  chunks.reserve(20);
							 }
				    		 
				    		 
				    		 //loadChunk(sf::Vector2f(x, y));
				    	 }
						 if (async)
						 {
							 std::unique_lock<std::mutex> lock(m_mutex);

							 updateGeneratedChunkBounds(chunks);
							 for (size_t ch = 0; ch < chunks.size(); ++ch)
								 insertNewChunk(chunks[ch]);
						 }
						 else
						 {
							 std::unique_lock<std::mutex> lock(m_mutex);
							 chunkLoader->loadedChunks.insert(chunkLoader->loadedChunks.end(), chunks.begin(), chunks.end());
						 }
						 m_threadsLoading--;
						 
				     }));
				}

				

				if (async)
				{
					
				}
				else
				{
					for (auto& thread : chunkLoader->threads)
					{
						thread->join();
						delete thread;
					}
					updateGeneratedChunkBounds(chunkLoader->loadedChunks);
					for (auto& ch : chunkLoader->loadedChunks)
						insertNewChunk(ch);
					delete chunkLoader;
				}
			}

		}
		void ChunkManager::loadChunk(const sf::Vector2f& pos)
		{
			QSFMLP_GENERAL_FUNCTION(QSFML_COLOR_STAGE_1);
			sf::Vector2i chunkPos = getChunkPosition(pos);
			bool exists = true;
			{
				if (m_threadsLoading > 0)
				{
					std::unique_lock<std::mutex> lock(m_mutex);
					auto it = m_loadedChunks.find(chunkPos);
					if (it == m_loadedChunks.end())
					{
						exists = false;
					}
				}
				else
				{
					auto it = m_loadedChunks.find(chunkPos);
					if (it == m_loadedChunks.end())
					{
						exists = false;
					}
				}
				
			}
			if (!exists)
			{
#ifdef QSFML_DEBUG
				getLogger().logInfo("loadChunk(): Pos: {" + std::to_string(chunkPos.x) + "," + std::to_string(chunkPos.y) + "} "
									"Loading new chunk");
#endif
				Chunk* chunk = m_chunkFactory->create(chunkPos, m_resources, m_scale);
				//Chunk* chunk = new Chunk(chunkPos, m_resources, m_scale);
				chunk->generate();
				chunk->updateTextureCoords();
				

				updateGeneratedChunkBounds({ chunk });
				insertNewChunk(chunk);


			}
			else
			{
				getLogger().logInfo("loadChunk(): Pos: {" + std::to_string(chunkPos.x) + "," + std::to_string(chunkPos.y) + "} "
									"Chunk already loaded");
			}
		}


		const std::vector<Chunk*>& ChunkManager::getChunks(const sf::FloatRect& area) const
		{
			QSFMLP_GENERAL_FUNCTION(QSFML_COLOR_STAGE_1);
#ifdef QSFML_DEBUG
			m_visibleChunkGroupBounds.clear();
#endif
			if (!m_needsDrawUpdate && area == m_visibleArea)
			{
				return m_visibleChunks;
			}
			m_visibleArea = area;
			size_t lastSize = m_visibleChunks.size();
			m_visibleChunks.clear();
			

			sf::Vector2f realPosStart = sf::Vector2f(area.left / m_scale, area.top / m_scale);
			sf::Vector2f realPosEnd = sf::Vector2f((area.left + area.width) / m_scale, (area.top + area.height) / m_scale);

			sf::Vector2i areaStart = getChunkGroupPosition(realPosStart);
			sf::Vector2i areaEnd = getChunkGroupPosition(realPosEnd);

			sf::Vector2i relativeStart = getRelativeChunkPosInGroup(realPosStart);
			sf::Vector2i relativeStop  = getRelativeChunkPosInGroup(realPosEnd);

			// Take min range from ChunkBounds
			sf::Vector2i chunkGroupStartBound = getChunkGroupPosition(sf::Vector2f(m_generatedChunkBounds.left, m_generatedChunkBounds.top));
			sf::Vector2i chunkGroupEndBound = getChunkGroupPosition(sf::Vector2f(m_generatedChunkBounds.left + m_generatedChunkBounds.width, m_generatedChunkBounds.top + m_generatedChunkBounds.height));
			//sf::Vector2i chunkGroupStart(std::max(areaStart.x, chunkGroupStartBound.x), std::max(areaStart.y, chunkGroupStartBound.y));
			//sf::Vector2i chunkGroupEnd(std::max(areaEnd.x, chunkGroupEndBound.x), std::max(areaEnd.y, chunkGroupEndBound.y));
			//sf::Vector2i chunkGroupStart(areaStart.x, areaStart.y);
			//sf::Vector2i chunkGroupEnd(areaEnd.x, areaEnd.y);

			sf::Vector2i chunkGroupStart(std::max(areaStart.x, m_generatedChunkBounds.left), std::max(areaStart.y, m_generatedChunkBounds.top));
			sf::Vector2i chunkGroupEnd(std::min(areaEnd.x, m_generatedChunkBounds.left + m_generatedChunkBounds.width), std::min(areaEnd.y, m_generatedChunkBounds.top + m_generatedChunkBounds.height));

			chunkGroupStart = getChunkGroupPosition(chunkGroupStart);
			chunkGroupEnd = getChunkGroupPosition(chunkGroupEnd);

			if (chunkGroupStart.x > chunkGroupEnd.x || chunkGroupStart.y > chunkGroupEnd.y)
			{
				return m_visibleChunks;
			}


			m_visibleChunks.reserve(lastSize * 2);
			m_visibleChunkGroups.clear();
			m_visibleChunkGroups.reserve(16);

			// Find relevant ChunkGroups
			for (int x = chunkGroupStart.x; x <= chunkGroupEnd.x; x += Chunk::CHUNK_SIZE_SQR)
			{
				for (int y = chunkGroupStart.y; y <= chunkGroupEnd.y; y+= Chunk::CHUNK_SIZE_SQR)
				{
					QSFMLP_GENERAL_BLOCK("Find chunk group", QSFML_COLOR_STAGE_2);
					const auto it = m_chunkGroups.find(sf::Vector2i(x, y));
					if (it != m_chunkGroups.end())
					{
						m_visibleChunkGroups.push_back(it->second);
					}
				}
			}


			relativeStop.x = std::min(relativeStop.x, Chunk::CHUNK_SIZE);
			relativeStop.y = std::min(relativeStop.y, Chunk::CHUNK_SIZE);
			// Search in chunk groups
			for (auto& group : m_visibleChunkGroups)
			{
				sf::Vector2i chunkGroupPos = group->position;
			
				sf::Vector2i startIndex(0, 0);
				sf::Vector2i endIndex(Chunk::CHUNK_SIZE - 1, Chunk::CHUNK_SIZE - 1);
			
				if (areaStart.x >= chunkGroupPos.x)
				{
					startIndex.x = relativeStart.x;
				}
				if (areaStart.y >= chunkGroupPos.y)
				{
					startIndex.y = relativeStart.y;
				}
				if (areaEnd.x <= chunkGroupPos.x)
				{
					endIndex.x = relativeStop.x;
				}
				if (areaEnd.y <= chunkGroupPos.y)
				{
					endIndex.y = relativeStop.y;
				}
			
#ifdef QSFML_DEBUG
				sf::FloatRect gridRect = sf::FloatRect(sf::Vector2f(chunkGroupPos) + sf::Vector2f(startIndex * Chunk::CHUNK_SIZE), 
													   sf::Vector2f((endIndex.x - startIndex.x+1)* Chunk::CHUNK_SIZE, (endIndex.y - startIndex.y+1) * Chunk::CHUNK_SIZE));
				m_visibleChunkGroupBounds.push_back(gridRect);
#endif
			
				for (int x = startIndex.x; x <= endIndex.x; x++)
				{
					for (int y = startIndex.y; y <= endIndex.y; y++)
					{
						QSFMLP_GENERAL_BLOCK("Find chunk", QSFML_COLOR_STAGE_2);
						Chunk* chunk = group->getChunk(x, y);
						if (chunk)
						{
							m_visibleChunks.push_back(chunk);
						}
					}
				}
			}

			
		/*
			sf::Vector2i start = sf::Vector2i(((int)(Chunk::CHUNK_SIZE * area.left / m_scale) / (int)Chunk::CHUNK_SIZE),
											  ((int)(Chunk::CHUNK_SIZE * area.top / m_scale) / (int)Chunk::CHUNK_SIZE));

			sf::Vector2i end = sf::Vector2i(((int)(Chunk::CHUNK_SIZE * (area.left + area.width) / m_scale) / (int)Chunk::CHUNK_SIZE),
											((int)(Chunk::CHUNK_SIZE * (area.top + area.height) / m_scale) / (int)Chunk::CHUNK_SIZE));

			for (int x = start.x; x <= end.x; x++)
			{
				for (int y = start.y; y <= end.y; y++)
				{
					QSFMLP_GENERAL_BLOCK("Find chunk", QSFML_COLOR_STAGE_2);
					auto it = m_loadedChunks.find(sf::Vector2i(x, y));
					if (it != m_loadedChunks.end())
					{
						m_visibleChunks.push_back(it->second);
					}
				}
			}*/
			m_needsDrawUpdate = false;
			return m_visibleChunks;
		}

		const std::vector<ChunkManager::ChunkGroup*>& ChunkManager::getChunkGroups(const sf::FloatRect& area) const
		{
#ifdef QSFML_DEBUG
			m_visibleChunkGroupBounds.clear();
#endif
			size_t lastSize = m_visibleChunkGroups.size();
			m_visibleChunkGroups.clear();
			m_visibleChunkGroups.reserve(lastSize*2);

			sf::Vector2f realPosStart = sf::Vector2f(area.left / m_scale, area.top / m_scale);
			sf::Vector2f realPosEnd = sf::Vector2f((area.left + area.width) / m_scale, (area.top + area.height) / m_scale);

			
			sf::Vector2i areaStart = getChunkGroupPosition(realPosStart);
			sf::Vector2i areaEnd = getChunkGroupPosition(realPosEnd);

			sf::Vector2i chunkGroupStart(std::max(areaStart.x, m_generatedChunkBounds.left), std::max(areaStart.y, m_generatedChunkBounds.top));
			sf::Vector2i chunkGroupEnd(std::min(areaEnd.x, m_generatedChunkBounds.left+ m_generatedChunkBounds.width), std::min(areaEnd.y, m_generatedChunkBounds.top+ m_generatedChunkBounds.height));

			chunkGroupStart = getChunkGroupPosition(chunkGroupStart);
			chunkGroupEnd = getChunkGroupPosition(chunkGroupEnd);

			if (chunkGroupStart.x > chunkGroupEnd.x || chunkGroupStart.y > chunkGroupEnd.y)
			{
				return m_visibleChunkGroups;
			}
			// Take min range from ChunkBounds
			//sf::Vector2i chunkGroupStart(areaStart.x, areaStart.y);
			//sf::Vector2i chunkGroupEnd(areaEnd.x, areaEnd.y);

#ifdef QSFML_DEBUG
			sf::FloatRect frameRect = sf::FloatRect(sf::Vector2f(chunkGroupStart),
												   sf::Vector2f(chunkGroupEnd- chunkGroupStart));
			m_visibleChunkGroupBounds.push_back(frameRect);
#endif

			// Find relevant ChunkGroups
			for (int x = chunkGroupStart.x; x <= chunkGroupEnd.x; x += Chunk::CHUNK_SIZE_SQR)
			{
				for (int y = chunkGroupStart.y; y <= chunkGroupEnd.y; y += Chunk::CHUNK_SIZE_SQR)
				{
					QSFMLP_GENERAL_BLOCK("Find chunk group", QSFML_COLOR_STAGE_2);
					const auto it = m_chunkGroups.find(sf::Vector2i(x, y));
					if (it != m_chunkGroups.end())
					{
						ChunkGroup* group = it->second;
						m_visibleChunkGroups.push_back(group);


#ifdef QSFML_DEBUG
						sf::FloatRect gridRect = sf::FloatRect(sf::Vector2f(group->position),
							sf::Vector2f(Chunk::CHUNK_SIZE_SQR, Chunk::CHUNK_SIZE_SQR));
						m_visibleChunkGroupBounds.push_back(gridRect);
#endif

					}
				}
			}
			return m_visibleChunkGroups;
		}

		void ChunkManager::updateGeneratedChunkBounds(const QSFML::vector<Chunk*>& newChunks)
		{
			// Create rect surrounding all chunks
			m_generatedChunkBounds = Chunk::getBounds(m_generatedChunkBounds, Chunk::getBounds(newChunks));
			return;

			sf::IntRect bounds;
			for (auto& chunk : newChunks)
			{
				sf::IntRect chunkBounds = chunk->getBounds();

				if (chunkBounds.left < bounds.left)
				{
					bounds.width += bounds.left - chunkBounds.left;
					bounds.left = chunkBounds.left;
				}
				if (chunkBounds.top < bounds.top)
				{
					bounds.height += bounds.top - chunkBounds.top;
					bounds.top = chunkBounds.top;
				}
				if (chunkBounds.left + chunkBounds.width > bounds.left + bounds.width)
				{
					bounds.width = chunkBounds.left + chunkBounds.width - bounds.left;
				}
				if (chunkBounds.top + chunkBounds.height > bounds.top + bounds.height)
				{
					bounds.height = chunkBounds.top + chunkBounds.height - bounds.top;
				}
			}

			// Compare the bounds to the generated bounds
			if (bounds.left < m_generatedChunkBounds.left)
			{
				m_generatedChunkBounds.width += m_generatedChunkBounds.left - bounds.left;
				m_generatedChunkBounds.left = bounds.left;
			}
			else if (bounds.left + bounds.width >= m_generatedChunkBounds.left + m_generatedChunkBounds.width)
			{
				m_generatedChunkBounds.width = bounds.left - m_generatedChunkBounds.left + bounds.width;
			}
		}
		void ChunkManager::updateGeneratedChunkBounds(const Chunk*& newChunk)
		{
			m_generatedChunkBounds = Chunk::getBounds(m_generatedChunkBounds, newChunk->getBounds());
		}
		void ChunkManager::insertNewChunk(Chunk*& chunk)
		{
			sf::Vector2i chunkPos = chunk->getPosition();
			sf::Vector2i chunkGroupPos = getChunkGroupPosition(chunkPos);

			m_loadedChunks.insert(std::make_pair(chunkPos, chunk));
			m_loadedChunksVec.push_back(chunk);

			auto groupIt = m_chunkGroups.find(chunkGroupPos);
			if (groupIt == m_chunkGroups.end())
			{
				ChunkGroup* group = new ChunkGroup(chunkGroupPos);
				group->chunks[getRelativeChunkIndexInGroup(getRelativeChunkPosInGroup(chunkPos))] = chunk;
				group->updateLowResMap();
				m_chunkGroups.insert(std::make_pair(chunkGroupPos, group));
			}
			else
			{
				ChunkGroup* group = groupIt->second;
				group->chunks[getRelativeChunkIndexInGroup(getRelativeChunkPosInGroup(chunkPos))] = chunk;
				group->updateLowResMap();
			}
			m_needsDrawUpdate = true;
		}

		void ChunkManager::draw(sf::RenderTarget& target, sf::RenderStates states) const
		{
			const sf::View& view = target.getView();
			float zoom = view.getSize().x / target.getSize().x;
			//float rotation = view.getRotation();

			 sf::FloatRect viewRect = sf::FloatRect(view.getCenter() - view.getSize() / 2.f, view.getSize());
			 // make the view bigger to match the rotation of the rect
			 sf::Vector2f size = view.getSize();
			 //float angle = rotation * 3.14159265359 / 180.f;
			 float diag = sqrt(size.x * size.x + size.y * size.y);
			 sf::Vector2f newSize = sf::Vector2f(diag, diag);
			 sf::Vector2f center = view.getCenter();
			 //sf::Vector2f offset = sf::Vector2f(diag - size.x, diag - size.y) / 2.f;
			 viewRect = sf::FloatRect(center - newSize / 2.f, newSize);


#ifdef QSFML_DEBUG
			float width = viewRect.width;
			float height = viewRect.height;
			float padding = 0.3;
			viewRect.left += width * padding;
			viewRect.top += height * padding;
			viewRect.width -= width * 2 * padding;
			viewRect.height -= height * 2 * padding;

			// Set GL line width
			glLineWidth(3);
#endif
			if (zoom < 1)
			{
				const std::vector<Chunk*>& chunks = getChunks(viewRect);
				if (zoom < 0.2)
				{
					states.texture = &m_resources.textureMap.getTexture();
					for (auto& chunk : chunks)
					{
						chunk->draw_LOD_high(target, states);
					}
				}
				else
				{
					for (auto& chunk : chunks)
					{
						chunk->draw_LOD_low(target, states);
					}
				}
				//getLogger().logInfo("Visible chunks: " + std::to_string(chunks.size()));
			}
			else
			{
				getChunkGroups(viewRect);
				for (auto& chunk : m_visibleChunkGroups)
				{
					chunk->draw(target, states);
				}
				//getLogger().logInfo("Visible low level chunk groups: " + std::to_string(m_visibleChunkGroups.size()));
			}

#ifdef QSFML_DEBUG
			glLineWidth(3);

			for (const ChunkGroup* group : m_visibleChunkGroups)
			{
				const sf::Vector2i& pos = group->position;
				sf::Vector2i end = pos + sf::Vector2i(Chunk::CHUNK_SIZE_SQR, Chunk::CHUNK_SIZE_SQR);
				// Draw chunkGroup lines
				sf::VertexArray lines(sf::PrimitiveType::LineStrip, 5);
				lines[0].position = sf::Vector2f(pos);
				lines[1].position = sf::Vector2f(end.x, pos.y);
				lines[2].position = sf::Vector2f(end);
				lines[3].position = sf::Vector2f(pos.x, end.y);
				lines[4].position = lines[0].position;

				sf::Color color(0, 0, 255);
				lines[0].color = color;
				lines[1].color = color;
				lines[2].color = color;
				lines[3].color = color;
				lines[4].color = color;
				target.draw(lines);
			}
			glLineWidth(2);
			for (const sf::FloatRect& rect : m_visibleChunkGroupBounds)
			{
				sf::VertexArray lines(sf::PrimitiveType::LineStrip, 5);
				lines[0].position = sf::Vector2f(rect.left, rect.top);
				lines[1].position = sf::Vector2f(rect.left + rect.width, rect.top);
				lines[2].position = sf::Vector2f(rect.left + rect.width, rect.top + rect.height);
				lines[3].position = sf::Vector2f(rect.left, rect.top + rect.height);
				lines[4].position = lines[0].position;

				sf::Color color(200, 255, 0);
				lines[0].color = color;
				lines[1].color = color;
				lines[2].color = color;
				lines[3].color = color;
				lines[4].color = color;
				target.draw(lines);
			}
			glLineWidth(1);
			{
				sf::VertexArray lines(sf::PrimitiveType::LineStrip, 5);
				lines[0].position = sf::Vector2f(viewRect.left, viewRect.top);
				lines[1].position = sf::Vector2f(viewRect.left + viewRect.width, viewRect.top);
				lines[2].position = sf::Vector2f(viewRect.left + viewRect.width, viewRect.top + viewRect.height);
				lines[3].position = sf::Vector2f(viewRect.left, viewRect.top + viewRect.height);
				lines[4].position = lines[0].position;

				sf::Color color(200, 200, 200);
				lines[0].color = color;
				lines[1].color = color;
				lines[2].color = color;
				lines[3].color = color;
				lines[4].color = color;
				target.draw(lines);
			}
			{
				sf::VertexArray lines(sf::PrimitiveType::LineStrip, 5);
				lines[0].position = sf::Vector2f(m_generatedChunkBounds.left, m_generatedChunkBounds.top);
				lines[1].position = sf::Vector2f(m_generatedChunkBounds.left + m_generatedChunkBounds.width, m_generatedChunkBounds.top);
				lines[2].position = sf::Vector2f(m_generatedChunkBounds.left + m_generatedChunkBounds.width, m_generatedChunkBounds.top + m_generatedChunkBounds.height);
				lines[3].position = sf::Vector2f(m_generatedChunkBounds.left, m_generatedChunkBounds.top + m_generatedChunkBounds.height);
				lines[4].position = lines[0].position;

				sf::Color color(200, 200, 200);
				lines[0].color = color;
				lines[1].color = color;
				lines[2].color = color;
				lines[3].color = color;
				lines[4].color = color;
				target.draw(lines);
			}
			
#endif
			
		}





		ChunkManager::ChunkGroup::ChunkGroup(const sf::Vector2i& pos)
			: position(pos)
			, m_vertexBuffer(sf::PrimitiveType::Quads, Chunk::CHUNK_SIZE_SQR * 4)
		{
			memset(chunks, 0, sizeof(Chunk*) * Chunk::CHUNK_SIZE * Chunk::CHUNK_SIZE);
			float scale = Chunk::CHUNK_SIZE;

			const sf::Color& color = sf::Color::Transparent;
			for (int x = 0; x < Chunk::CHUNK_SIZE; x++)
			{
				for (int y = 0; y < Chunk::CHUNK_SIZE; y++)
				{
					size_t idx = (y * Chunk::CHUNK_SIZE + x) * 4;
					int xCoord = x * scale;
					int yCoord = y * scale;
					//	sf::Vertex* quad = &m_vertexBuffer[idx];
					m_vertexBuffer[idx].position = sf::Vector2f(xCoord + position.x, yCoord + position.y);
					m_vertexBuffer[idx + 1].position = sf::Vector2f(xCoord + scale + position.x, yCoord + position.y);
					m_vertexBuffer[idx + 2].position = sf::Vector2f(xCoord + scale + position.x, yCoord + scale + position.y);
					m_vertexBuffer[idx + 3].position = sf::Vector2f(xCoord + position.x, yCoord + scale + position.y);

					m_vertexBuffer[idx].color = color;
					m_vertexBuffer[idx + 1].color = color;
					m_vertexBuffer[idx + 2].color = color;
					m_vertexBuffer[idx + 3].color = color;
				}
			}
		}

		void ChunkManager::ChunkGroup::updateLowResMap()
		{
			for (size_t x = 0; x < Chunk::CHUNK_SIZE; x++)
			{
				for (size_t y = 0; y < Chunk::CHUNK_SIZE; y++)
				{
					Chunk* chunk = chunks[y * Chunk::CHUNK_SIZE + x];
					if (chunk)
					{
						const sf::Color& color = chunk->getChunkData()->getLowLevelReplacementColor();
						size_t idx = (y * Chunk::CHUNK_SIZE + x) * 4;
						m_vertexBuffer[idx].color = color;
						m_vertexBuffer[idx + 1].color = color;
						m_vertexBuffer[idx + 2].color = color;
						m_vertexBuffer[idx + 3].color = color;
					}
				}
			}

		}
	}
}