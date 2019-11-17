#pragma once

#include <string>
#include <algorithm>
#include <fstream>

#include <thread>

namespace Framework
{
	namespace Profile
	{
		struct ProfileResult
		{
			std::string Name;
			long long Start, End;
			uint32_t ThreadID;
		};

		struct InstrumentationSession
		{
			std::string Name;
		};

		class Instrumentor
		{
		public:
			Instrumentor();
			~Instrumentor();

			void BeginSession(const std::string a_name, const std::string& a_filepath = "results.json");
			void EndSession();

			static Instrumentor& Get();

			void WriteProfile(const ProfileResult& a_result);

		private:
			void WriteHeader();
			void WriteFooter();

		private:
			InstrumentationSession* m_currentSession;
			std::ofstream m_outputStream;
			int m_profileCount;
		};

		class InstrumentationTimer
		{
		public:
			InstrumentationTimer(const char* name);

			~InstrumentationTimer();

			void Stop();
		private:
			const char* m_Name;
			std::chrono::time_point<std::chrono::steady_clock> m_StartTimepoint;
			bool m_Stopped;
		};
	}
}

#define PROFILING 1
#if PROFILING
#define PROFILE_SCOPE(name) Framework::Profile::InstrumentationTimer timer##__LINE__(name)
#define PROFILE_FUNCTION() PROFILE_SCOPE(__FUNCTION__)
#else
#define PROFILE_SCOPE(name)
#endif