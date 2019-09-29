#include "FWTime.h"

#include <GLFW/glfw3.h>

namespace Framework
{
	std::chrono::high_resolution_clock Time::clock = std::chrono::high_resolution_clock();
	std::chrono::steady_clock::time_point Time::m_tpCurrentTime = std::chrono::steady_clock::time_point();

	 double Time::m_prevTime = 0.0;
	 double Time::m_currentTime = 0.0;
	 double Time::m_deltaTime = 0.0;
	 double Time::m_totalTime = 0.0;

	void Time::UpdateTime()
	{
		m_tpCurrentTime = clock.now();
		m_currentTime = std::chrono::duration<double>(m_tpCurrentTime.time_since_epoch()).count(); //glfwGetTime();

		m_deltaTime = m_currentTime - m_prevTime;

		m_prevTime = m_currentTime;
	}

	float Time::GetDeltaTime()
	{
		return (float)m_deltaTime;
	}

	float Time::GetTotalTime()
	{
		return (float)m_totalTime;
	}
}