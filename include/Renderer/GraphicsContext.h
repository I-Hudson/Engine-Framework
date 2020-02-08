#pragma once

#include <string>
#include "Window.h"

namespace Framework
{
	namespace Renderer
	{
		class GraphicsContext
		{
		public:
			virtual void Init(const int& a_width, const int& a_height, const std::string& a_title, const bool& a_fullscreen, void* a_window, Window::WindowData* a_windowData) = 0;
			virtual void Destroy() = 0;

			virtual void SwapBuffers() = 0;

			virtual void* GetNativeContext() = 0;
			virtual void* GetWindow() = 0;

			static GraphicsContext* Create(const int& a_width, const int& a_height, const std::string& a_title,
				const bool& a_fullscreen = false, void* a_window = nullptr, Window::WindowData* a_windowData = &Window::WindowData());

		protected:
			static GraphicsContext* s_instance;
		};
	}
}