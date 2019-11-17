#include "Profile/Instrumentor.h"

namespace Framework
{
	namespace Profile
	{
		Instrumentor::Instrumentor()
		{
		}

		Instrumentor::~Instrumentor()
		{
		}

		void Instrumentor::BeginSession(const std::string a_name, const std::string& a_filepath)
		{
			m_outputStream.open(a_filepath);
			WriteHeader();
			m_currentSession = new InstrumentationSession{ a_name };
		}

		void Instrumentor::EndSession()
		{
			WriteFooter();
			m_outputStream.close();
			delete m_currentSession;
			m_currentSession = nullptr;
			m_profileCount = 0;
		}

		Instrumentor& Instrumentor::Get()
		{
			static Instrumentor* instance = new Instrumentor();
			return *instance;
		}

		void Instrumentor::WriteProfile(const ProfileResult& a_result)
		{
			if (m_profileCount++ > 0)
			{
				m_outputStream << ",";
			}

			std::string name = a_result.Name;
			std::replace(name.begin(), name.end(), '"', '\'');

			m_outputStream << "{";
			m_outputStream << "\"cat\":\"function\",";
			m_outputStream << "\"dur\":" << (a_result.End - a_result.Start) << ',';
			m_outputStream << "\"name\":\"" << name << "\",";
			m_outputStream << "\"ph\":\"X\",";
			m_outputStream << "\"pid\":0,";
			m_outputStream << "\"tid\":" << a_result.ThreadID << ",";
			m_outputStream << "\"ts\":" << a_result.Start;
			m_outputStream << "}";

			m_outputStream.flush();
		}

		void Instrumentor::WriteHeader()
		{
			m_outputStream << "{\"otherData\": {},\"traceEvents\":[";
			m_outputStream.flush();
		}

		void Instrumentor::WriteFooter()
		{
			m_outputStream << "]}";
			m_outputStream.flush();
		}


		InstrumentationTimer::InstrumentationTimer(const char* name)
			: m_Name(name), m_Stopped(false)
		{
			m_StartTimepoint = std::chrono::high_resolution_clock::now();
		}

		InstrumentationTimer::~InstrumentationTimer()
		{
			if (!m_Stopped)
				Stop();
		}

		void InstrumentationTimer::Stop()
		{
			auto endTimepoint = std::chrono::high_resolution_clock::now();

			long long start = std::chrono::time_point_cast<std::chrono::microseconds>(m_StartTimepoint).time_since_epoch().count();
			long long end = std::chrono::time_point_cast<std::chrono::microseconds>(endTimepoint).time_since_epoch().count();

			uint32_t threadID = std::hash<std::thread::id>{}(std::this_thread::get_id());
			Instrumentor::Get().WriteProfile({ m_Name, start, end, threadID });

			m_Stopped = true;
		}
	}
}