#include "utilities/LifetimeChecker.h"

namespace QSFML
{
	namespace Internal
	{
		LifetimeChecker::LifetimeChecker()
		{

		}
		LifetimeChecker::~LifetimeChecker()
		{

		}

		LifetimeChecker& LifetimeChecker::instance()
		{
			static LifetimeChecker instance;
			return instance;
		}

		QSFML::vector<void*> LifetimeChecker::getAlivePointers()
		{
			LifetimeChecker& inst = instance();
			std::lock_guard<std::mutex> lock(inst.m_mutex);
			QSFML::vector<void*> alive;
			for (auto& pair : inst.m_alive)
			{
				if (pair.second && pair.first)
					alive.push_back(pair.first);
			}
			return alive;
		}
		size_t LifetimeChecker::getAliveCount()
		{
			LifetimeChecker& inst = instance();
			std::lock_guard<std::mutex> lock(inst.m_mutex);
			size_t count = 0;
			for (auto& pair : inst.m_alive)
			{
				if (pair.second)
					++count;
			}
			return count;
		}
		size_t LifetimeChecker::getDeadCount()
		{
			LifetimeChecker& inst = instance();
			std::lock_guard<std::mutex> lock(inst.m_mutex);
			size_t count = 0;
			for (auto& pair : inst.m_alive)
			{
				if (!pair.second)
					++count;
			}
			return count;
		}
		size_t LifetimeChecker::getTotalCount()
		{
			LifetimeChecker& inst = instance();
			std::lock_guard<std::mutex> lock(inst.m_mutex);
			return inst.m_alive.size();
		}


		void LifetimeChecker::shrink()
		{
			LifetimeChecker& inst = instance();
			std::lock_guard<std::mutex> lock(inst.m_mutex);
			QSFML::vector<void*> stillAlive;
			stillAlive.reserve(inst.m_alive.size());
			for (auto it = inst.m_alive.begin(); it != inst.m_alive.end();)
			{
				if (it->second && it->first)
					stillAlive.push_back(it->first);
			}
			inst.m_alive.clear();
			for (auto ptr : stillAlive)
				inst.m_alive[ptr] = true;
		}
		void LifetimeChecker::add(void* ptr)
		{
			LifetimeChecker& inst = instance();
			if (!ptr)
				return;
			std::lock_guard<std::mutex> lock(inst.m_mutex);
			inst.m_alive[ptr] = true;
		}
		bool LifetimeChecker::isAlive(void* ptr)
		{
			LifetimeChecker &inst = instance();
			if (!ptr)
				return false;
			std::lock_guard<std::mutex> lock(inst.m_mutex);
			bool isAlive = inst.isAlive_internal(ptr);
			if (!isAlive)
			{
				int a = 0;
				a;
			}
			return isAlive;
		}
		void LifetimeChecker::setDead(void* ptr)
		{
			LifetimeChecker& inst = instance();
			if (!ptr)
				return;
			std::lock_guard<std::mutex> lock(inst.m_mutex);
			if (inst.m_alive.find(ptr) == inst.m_alive.end())
				return;
			inst.m_alive[ptr] = false;
		}
		bool LifetimeChecker::exists(void* ptr)
		{
			LifetimeChecker& inst = instance();
			if (!ptr)
				return false;
			std::lock_guard<std::mutex> lock(inst.m_mutex);
			return inst.m_alive.find(ptr) != inst.m_alive.end();
		}


		bool LifetimeChecker::isAlive_internal(void* ptr)
		{
			if (!ptr)
				return false;
			if (m_alive.find(ptr) == m_alive.end())
				return false;
			return m_alive[ptr];
		}
		void LifetimeChecker::setDead_internal(void* ptr)
		{
			if (!ptr)
				return;
			if (m_alive.find(ptr) == m_alive.end())
				return;
			m_alive[ptr] = false;
		}
	}
}