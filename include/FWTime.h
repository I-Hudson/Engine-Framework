#pragma once

#include <chrono>

namespace Framework
{
	class Time
	{
	public:
		static void UpdateTime();
		static float GetDeltaTime();
		static float GetDeltaTimeRaw();
		static float GetTotalTime();

		static float GetTimeScale();
		static float SetTimeScale(const float& aTimeScale);

	private:
		static std::chrono::high_resolution_clock clock;
		static std::chrono::steady_clock::time_point m_tpCurrentTime;

		static double m_prevTime;
		static double m_currentTime;
		static double m_deltaTime;
		static double m_totalTime;

		static float m_timeScale;
	};
}