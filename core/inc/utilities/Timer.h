#pragma once

#include "QSFML_EditorWidget_base.h"

#include <functional>

namespace QSFML
{
	namespace Utilities
	{
		class QSFML_EDITOR_WIDGET_EXPORT Timer
		{
		public:
			Timer();
			~Timer();

			bool update();
			bool start(double seconds);
			bool start();
			void setInterval(double seconds);
			double stop();

			void onFinished(const std::function<void(void)> &func);
			double runtime();

			void autoRestart(bool autoRestart);
			bool isRunning();


		private:
			//void reset();

			//size_t benoetigteZeit
			//size_t vergangeneZeit;
			bool m_autorestart;
			TimePoint m_t1;
			double m_targetSeconds;
			bool m_running;
			std::function<void(void)>* m_func;
		};
	}
}
