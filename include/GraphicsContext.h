#pragma once

#include <string>
#include <memory>
#include <Windows.h>

namespace Framework
{
	class GraphicsContext
	{
	public:
		virtual void Init(const int& a_width, const int& a_height, const std::string& a_title, const bool& a_fullscreen) = 0;

		virtual void SwapBuffers() = 0;

		virtual void* GetNativeContext() = 0;

		static std::shared_ptr<GraphicsContext> Create(const int& a_width, const int& a_height, const std::string& a_title, const bool& a_fullscreen = false);
		static std::shared_ptr<GraphicsContext> Create(const int& a_width, const int& a_height, const std::string& a_title, const bool& a_fullscreen,
														HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow);
	};
}