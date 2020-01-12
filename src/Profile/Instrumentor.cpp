#include "Profile/Instrumentor.h"

#include <iostream>

namespace Profile
{
	Instrumentor::Instrumentor()
	{
	}

	Instrumentor::~Instrumentor()
	{
	}

	void to_json(json& a_json, const ProfileResult& a_profileresult)
	{
		a_json = json{
						{"cat", a_profileresult.Category},
						{"dur", a_profileresult.Duration},
						{"ph", a_profileresult.ph},
						{"pid", a_profileresult.pid},
						{"name", a_profileresult.Name},
						{"ts", a_profileresult.Start},
						{"te", a_profileresult.End},
						{"thr", a_profileresult.ThreadID}
		};
	}

	void from_json(const json& a_json, ProfileResult& a_profileresult)
	{
		a_json.at("cat").get_to(a_profileresult.Category);
		a_json.at("dur").get_to(a_profileresult.Duration);
		a_json.at("ph").get_to(a_profileresult.ph);
		a_json.at("pid").get_to(a_profileresult.pid);
		a_json.at("name").get_to(a_profileresult.Name);
		a_json.at("ts").get_to(a_profileresult.Start);
		a_json.at("te").get_to(a_profileresult.End);
		a_json.at("thr").get_to(a_profileresult.ThreadID);
	}

	void Instrumentor::BeginSession(const std::string a_name, const std::string& a_filepath)
	{
		m_outputStream.open(a_filepath);
		WriteHeader();
		m_allProfileResults = std::vector<json>();
		m_currentSession = new InstrumentationSession{ a_name };
	}

	void Instrumentor::EndSession()
	{
		WriteFooter();
		m_outputStream.close();
		delete m_currentSession;
		m_currentSession = nullptr;
		m_profileCount = 0;

		//std::ofstream jsonStream;
		//jsonStream.open("jsonFile.json");
		//
		//json j_vec(m_allProfileResult);
		//
		//jsonStream << j_vec << "\n";
		//jsonStream.flush();
		//jsonStream.close();
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
		m_outputStream << "\"te\":" << a_result.End;
		m_outputStream << "}\n";

		m_outputStream.flush();
	}

	void Instrumentor::WriteHeader()
	{
		m_outputStream << "{\"otherData\": {},\"traceEvents\":[\n";

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
		Instrumentor::Get().WriteProfile({ "funcation", 0, "", "", m_Name, start, end, threadID});

		m_Stopped = true;
	}
}