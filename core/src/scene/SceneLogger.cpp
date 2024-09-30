#include "scene/SceneLogger.h"

namespace QSFML
{
	namespace Internal
	{
		SceneLogger::SceneLogger()
			: m_sceneLogger("Scene")
			, m_objectLogger("Objects")
			, m_componentLogger("Components")
			, m_physicsLogger("Physics")
		{
			m_objectLogger.setParentID(m_sceneLogger.getID());
			m_componentLogger.setParentID(m_sceneLogger.getID());
			m_physicsLogger.setParentID(m_sceneLogger.getID());
		}
		SceneLogger::~SceneLogger()
		{
		
		}

		void SceneLogger::setColor(const Log::Color& col)
		{
			m_sceneLogger.setColor(col);
			m_objectLogger.setColor(col);
			m_componentLogger.setColor(col);
			m_physicsLogger.setColor(col);
		}

	}
}