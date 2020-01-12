#include "Log.h"

namespace Framework
{
	std::shared_ptr<spdlog::logger> Log::s_coreLogger = nullptr;
	std::shared_ptr<spdlog::logger> Log::s_clientLogger = nullptr;

	void Log::Init()
	{
		//spdlog::set_pattern("%^[%T] %n: %v%$");

		s_coreLogger = spdlog::stdout_color_st("Core");
		s_coreLogger->set_level(spdlog::level::trace);
		
		s_clientLogger = spdlog::stdout_color_mt("App");
		s_clientLogger->set_level(spdlog::level::trace);
	}

	void Log::Destroy()
	{
		s_coreLogger.reset();
		s_clientLogger.reset();
	}
}