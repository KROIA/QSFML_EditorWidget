#include "Events/DestroyEvent.h"

namespace QSFML
{
	namespace Events
	{
		DestroyEvent::DestroyEvent()
			: m_signal("DestroyEvent")
		{

		}

		DestroyEvent::~DestroyEvent()
		{
			m_signal.emitSignal();
		}

		void DestroyEvent::connect_onDestroy(const Slot::SlotFunction& slot)
		{
			m_signal.connectSlot(slot);
		}
		void DestroyEvent::disconnect_onDestroy(const Slot::SlotFunction& slot)
		{
			m_signal.disconnectSlot(slot);
		}
	}
}