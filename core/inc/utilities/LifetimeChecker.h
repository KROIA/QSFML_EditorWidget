#pragma once

#include "QSFML_EditorWidget_base.h"
#include <unordered_map>
#include <vector>
#include <string>
#include <iomanip>
#include <mutex>


namespace QSFML
{
	namespace Internal
	{
		class QSFML_EDITOR_WIDGET_EXPORT LifetimeChecker
		{
			LifetimeChecker();
			~LifetimeChecker();
		public:
			static LifetimeChecker &instance();

			static QSFML::vector<void*> getAlivePointers();
			static size_t getAliveCount();
			static size_t getDeadCount();
			static size_t getTotalCount();

			static void shrink();
			static void add(void* ptr);
			static bool isAlive(void* ptr);
			static void setDead(void* ptr);
			static bool exists(void* ptr);

			template<typename T>
			static void add(const T* ptr)
			{
				add((void*)ptr);
			}
			template<typename T>
			static bool isAlive(const T* ptr)
			{
				return isAlive((void*)ptr);
			}
			template<typename T>
			static void setDead(const T* ptr)
			{
				setDead((void*)ptr);
			}
			template<typename T>
			static bool exists(const T* ptr)
			{
				return exists((void*)ptr);
			}

			template<typename T>
			static void deleteSecured(T* ptr)
			{
				if(ptr == nullptr)
					return;
				LifetimeChecker& inst = instance();
				T* readyForDelete = nullptr;
				bool isAlive = false;
				{
					std::lock_guard<std::mutex> lock(inst.m_mutex);
					isAlive = inst.isAlive_internal(ptr);
					if (isAlive)
					{
						inst.setDead_internal(ptr);
						readyForDelete = ptr;
					}
				}
				delete readyForDelete;
				if(!isAlive)
				{
#ifdef LOGGER
					std::stringstream stream;
					stream << std::hex << (long long)ptr;
					std::string hexPtr(stream.str());
					std::string stringifiedTemplateName = typeid(T).name();
					Logger::logError("LifetimeChecker::deleteSecured: Attempt to delete a dead object: " + hexPtr + " TypeName:"+ stringifiedTemplateName);
#endif
				}
			}


		private:
			bool isAlive_internal(void* ptr);
			void setDead_internal(void* ptr);


			QSFML::unordered_map<void*, bool> m_alive;
			std::mutex m_mutex;
		};
	}
}