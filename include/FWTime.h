#pragma once

namespace Framework
{
	class Time
	{
	public:
		static void UpdateTime();
		static float GetDeltaTime();
		static float GetTotalTime();

	private:
		static double m_prevTime;
		static double m_currentTime;
		static double m_deltaTime;
		static double m_totalTime;
	};
}