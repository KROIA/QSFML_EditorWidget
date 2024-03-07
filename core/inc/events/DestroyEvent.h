#pragma once

#include "QSFML_base.h"
#include "utilities/Signal.h"

namespace QSFML
{
	namespace Events
	{
		/**
		*  @brief A class for destroying objects.
		*/
		class QSFML_EDITOR_WIDGET_EXPORT DestroyEvent
		{
		public:
			typedef Utilities::Signal<> Slot;
			DestroyEvent();
			~DestroyEvent();

			void connect_onDestroy(const Slot::SlotFunction& slot);
			template<typename ObjectType>
			void connect_onDestroy(ObjectType* obj, void(ObjectType::* memberFunc)())
			{
				m_signal.connectSlot(obj, memberFunc);
			}

			void disconnect_onDestroy(const Slot::SlotFunction& slot);
			template<typename ObjectType>
			void disconnect_onDestroy(ObjectType* obj, void(ObjectType::* memberFunc)())
			{
				m_signal.disconnectSlot(obj, memberFunc);
			}

			

		private:
			Slot m_signal;
		};
	}
}