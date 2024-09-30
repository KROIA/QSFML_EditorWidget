#pragma once
#include "QSFML_EditorWidget_base.h"
#include "Logger.h"

namespace QSFML
{
	namespace Internal
	{
		class QSFML_EDITOR_WIDGET_EXPORT SceneLogger
		{
			friend class Scene;
		private:
			SceneLogger();
			~SceneLogger();
		public:
			void setName(const std::string& name) { m_sceneLogger.setName(name); }
			void setColor(const Log::Color& col);

			void log(const Log::Message& msg){ m_sceneLogger.log(msg);}

			void log(const std::string& msg){ m_sceneLogger.log(msg);}
			void log(const std::string& msg, Log::Level level){ m_sceneLogger.log(msg, level);}
			void log(const std::string& msg, Log::Level level, const Log::Color& col){ m_sceneLogger.log(msg, level, col);}

			void logDebug(const std::string& msg){ m_sceneLogger.logDebug(msg);}
			void logInfo(const std::string& msg){ m_sceneLogger.logInfo(msg);}
			void logWarning(const std::string& msg){ m_sceneLogger.logWarning(msg);}
			void logError(const std::string& msg){ m_sceneLogger.logError(msg);}

			Log::LogObject& getSceneLogger(){ return m_sceneLogger; }
			Log::LogObject& getObjectLogger(){ return m_objectLogger; }
			Log::LogObject& getComponentLogger(){ return m_componentLogger; }
			Log::LogObject& getPhysicsLogger(){ return m_physicsLogger; }


		protected:
			
			
			
			Log::LogObject m_sceneLogger;
			Log::LogObject m_objectLogger;
			Log::LogObject m_componentLogger;
			Log::LogObject m_physicsLogger;

		};
	}
}
