#pragma once

#include "GraphicsContext.h"

struct GLFWwindow;

namespace Framework
{
	class OpenGLContext : public GraphicsContext
	{
	public:
		OpenGLContext(const int& a_width, const int& a_height, const std::string& a_title, const bool& a_fullscreen = false, Window::WindowData* a_windowData = &Window::WindowData());
		~OpenGLContext();

		virtual void Init(const int& a_width, const int& a_height, const std::string& a_title, const bool& a_fullscreen, void* a_window, Window::WindowData* a_windowData) override;
		virtual void Destroy() override;

		virtual void SwapBuffers() override;

		virtual void* GetNativeContext() override { return this; }
		GLFWwindow* GetWindow() { return m_window; }

	private:
		GLFWwindow* m_window;
	};
}