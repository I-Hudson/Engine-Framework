#include "FWTime.h"

#include <GLFW/glfw3.h>

namespace Framework
{
	 double Time::m_prevTime = 0.0;
	 double Time::m_currentTime = 0.0;
	 double Time::m_deltaTime = 0.0;
	 double Time::m_totalTime = 0.0;

	void Time::UpdateTime()
	{
		m_currentTime = glfwGetTime();
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