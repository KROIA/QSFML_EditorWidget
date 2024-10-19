#pragma once

#include "QSFML_EditorWidget_base.h"
#include "SFML/Graphics.hpp"
#include <QStringList>

namespace QSFML
{
	namespace Assets
	{
		/**
		 * @brief 
		 * This class is used to manage textures
		 * It loads all textures in a given path and stores them
		 * The textures can be accessed by their index or relative path
		 */
		class QSFML_EDITOR_WIDGET_EXPORT TextureManager
		{
			TextureManager();
			~TextureManager();
			static TextureManager& instance();
		public:

			/**
			 * @brief
			 * Defines a list of file filters to search for textures
			 * Format: "*.png"
			 * @param filters
			 */
			static void setFileNameFilters(const QStringList& filters)
			{
				instance().setFileNameFilters_internal(filters);
			}

			static const QStringList& getFileNameFilters()
			{
				return instance().m_fileNameFilters;
			}

			/**
			 * @brief
			 * Loads all textures in the Path "path"
			 * Searches recursively for all textures in the given path
			 * @warning Do not call this function multiple times
			 * @param path where to load the textures from
			 * @return true if all textures have been loaded successfully
			 */
			static bool loadTextures(const std::string& path)
			{
				return instance().loadTextures_internal(path);
			}

			/**
			 * @brief
			 * Checks if the textures have been loaded
			 * @return true if the textures have been loaded, otherwise false
			 */
			static bool isLoaded()
			{
				return instance().m_loaded;
			}

			/**
			 * @brief
			 * Checks if the texture at the given index has been loaded
			 * @param path of the texture, relative to the path given in loadTextures
			 * @return true if the texture has been loaded, otherwise false
			 */
			static bool isLoaded(const std::string& path)
			{
				return instance().isLoaded_internal(path);
			}

			/**
			 * @brief
			 * Gets the texture at the given index
			 * @param index of the texture
			 * @return Texture
			 */
			static const sf::Texture& getTexture(size_t index)
			{
				return instance().getTexture_internal(index);
			}

			/**
			 * @brief 
			 * Gets the texture at the given path
			 * @param path of the texture
			 * @return Texture
			 */
			static const sf::Texture& getTexture(const std::string& path)
			{
				return instance().getTexture_internal(path);
			}

			/**
			 * @brief
			 * Gets all textures
			 * @return a vetor of all textures
			 */
			static const QSFML::vector<sf::Texture>& getTextures()
			{
				return instance().m_textures;
			}

			/**
			 * @brief
			 * Gets all textures
			 * @return a map of all textures
			 */
			static const std::unordered_map<std::string, sf::Texture*>& getTextureMap()
			{
				return instance().m_textureMap;
			}

			/**
			 * @brief
			 * Gets the amount of loaded textures
			 * @return texture count
			 */
			static size_t getTextureCount()
			{
				return instance().m_textures.size();
			}

			/**
			 * @brief
			 * Clears all textures
			 * After calling this function, new textures can be loaded
			 */
			static void clear()
			{
				instance().clear_internal();
			}

		private:
			void setFileNameFilters_internal(const QStringList& filters)
			{
				m_fileNameFilters = filters;
			}

			bool loadTextures_internal(std::string path);
			const sf::Texture& getTexture_internal(size_t index)
			{
#ifdef QSFML_DEBUG
				if (index >= m_textures.size())
				{
					m_logger.logError("Texture index out of bounds: " + std::to_string(index));
					static sf::Texture empty;
					QSFML_DEBUG_BREAK();
					return empty;
				}
#endif
				return m_textures[index];
			}
			bool isLoaded_internal(const std::string& path)
			{
				return m_textureMap.find(path) != m_textureMap.end();
			}
			sf::Texture& getTexture_internal(const std::string& path)
			{
				const auto& it = m_textureMap.find(path);
				if (it != m_textureMap.end())
					return *it->second;
				m_logger.logError("Texture: \"" + path + "\" not found");
				static sf::Texture empty;
				return empty;
			}
			void clear_internal()
			{
				m_textures.clear();
				m_textureMap.clear();
				m_loaded = false;
#ifdef QSFML_DEBUG
				m_logger.logDebug("All textures cleared");
#endif
			}



			Log::LogObject m_logger;
			QStringList m_fileNameFilters;
			bool m_loaded = false;
			QSFML::vector<sf::Texture> m_textures;
			std::unordered_map<std::string, sf::Texture*> m_textureMap;


		};
	}
}