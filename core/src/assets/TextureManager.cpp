#include "assets/TextureManager.h"
#include <QDir>

namespace QSFML
{
	namespace Assets
	{
		TextureManager::TextureManager()
			: m_logger("TextureManager")
			, m_fileNameFilters({ "*.png", "*.jpg", "*.jpeg", "*.bmp" })
		{
#ifndef QSFML_DEBUG
			m_logger.setLevelEnabled(Log::Level::debug, false);
#endif
		}

		TextureManager::~TextureManager()
		{
			clear_internal();
		}

		TextureManager& TextureManager::instance()
		{
			static TextureManager instance;
			return instance;
		}

		bool TextureManager::loadTextures_internal(std::string path)
		{
			QSFMLP_ASSETS_FUNCTION(QSFML_COLOR_STAGE_1);
			bool success = true;
			if (m_loaded)
			{
				m_logger.logWarning("Textures have already been loaded");
				return false;
			}
			// Check if path exists and is a directory
			QDir dir(QString::fromStdString(path));
			if (!dir.exists())
			{
				m_logger.logError("Path does not exist: \"" + path + "\"");
				return false;
			}
			path = dir.absolutePath().toStdString();

			// Get a list of all image files in the given path recursively
#ifdef QSFML_DEBUG
			m_logger.logDebug("Loading textures from path: \"" + path + "\"");
#endif
			dir.setNameFilters(m_fileNameFilters);
			dir.setFilter(QDir::Files | QDir::NoDotAndDotDot | QDir::NoSymLinks);
			QFileInfoList list = dir.entryInfoList();

			// Reserve space for all textures
			m_textures.reserve(list.size());

			// Load all textures
			for (const QFileInfo& fileInfo : list)
			{
				QSFMLP_ASSETS_BLOCK(QSFML_COLOR_STAGE_2);
				sf::Texture texture;
				std::string texPath = fileInfo.absoluteFilePath().toStdString();
				std::string texName = texPath.substr(path.size() + 1);


				QSFMLP_ASSETS_TEXT("Filepath", texPath.c_str());

				if (!texture.loadFromFile(texPath))
				{
					m_logger.logError("Failed to load texture: \"" + texName + "\"");
					success = false;
					continue;
				}
				m_textures.push_back(std::move(texture));
				m_textureMap[texName] = &m_textures.back();
#ifdef QSFML_DEBUG
				int progress = (m_textures.size()*100) / list.size();
				m_logger.logDebug("[ " + std::to_string(progress) + "% ] Loaded texture : \"" + texName + "\"");
#endif
			}
			m_loaded = true;
			return success;
		}


	}
}