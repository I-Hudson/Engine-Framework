#pragma once

#include <string>
#include <memory>

namespace Framework
{
	class GraphicsContext
	{
	public:
		virtual void Init(const int& a_width, const int& a_height, const std::string& a_title, const bool& a_fullscreen) = 0;
		virtual void SwapBuffers() = 0;

		static std::shared_ptr<GraphicsContext> Create(const int& a_width, const int& a_height, const std::string& a_title, const bool& a_fullscreen = false);
	};
}