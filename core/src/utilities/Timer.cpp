#include "utilities/Timer.h"

namespace QSFML
{
	namespace Utilities
	{
		Timer::Timer()
		{
			m_func = nullptr;
			m_autorestart = false;
			m_running = false;
			m_targetSeconds = 0;
			//this->reset();
		}
		Timer::~Timer()
		{
			if (m_func)
				delete m_func;
		}
		/*void Timer::reset()
		{
			m_running = false;
			if (!m_autorestart)
			{
				
			}
			else
			{
				this->start(m_targetSeconds);
			}
		}*/
		bool Timer::update()
		{
			if (m_running)
			{
				TimePoint t2 = std::chrono::high_resolution_clock::now();
				std::chrono::duration<double> diff = t2 - m_t1;
				if ((diff.count() >= m_targetSeconds))
				{
					if (m_func)
					{
						(*m_func)();
					}
					if (m_autorestart)
					{
						m_t1 = std::chrono::high_resolution_clock::now();
					}
					else
						this->stop();
					return true;
				}
			}
			return false;
		}
		bool Timer::start()
		{
			if (!m_running)
			{
				m_running = true;
				m_t1 = std::chrono::high_resolution_clock::now();
			}
			if (this->update())
			{
				return true;
			}
			return false;
		}
		bool Timer::start(double seconds)
		{
			m_targetSeconds = seconds;
			return start();
		}
		void Timer::setInterval(double seconds)
		{
			m_targetSeconds = seconds;
		}
		double Timer::stop()
		{
			double runtime = this->runtime();
			m_running = false;
			//this->reset();
			return runtime;
		}
		void Timer::onFinished(const std::function<void(void)> &func)
		{
			if (m_func)
			{
				*m_func = func;
			}
			else
				m_func = new std::function<void(void)>(func);
		}
		double Timer::runtime()
		{
			TimePoint t2 = std::chrono::high_resolution_clock::now();
			std::chrono::duration<double> diff = t2 - m_t1;
			return diff.count();
		}
		void Timer::autoRestart(bool autoRestart)
		{
			m_autorestart = autoRestart;
		}
		bool Timer::isRunning()
		{
			return m_running;
		}
	}
}