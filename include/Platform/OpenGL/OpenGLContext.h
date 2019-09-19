#pragma once

#include "GraphicsContext.h"
#include <string>

struct GLFWwindow;

namespace Framework
{

	class OpenGLContext : public GraphicsContext
	{
	public:
		OpenGLContext(const int& a_width, const int& a_height, const std::string& a_title, const bool& a_fullscreen = false);
		~OpenGLContext();

		virtual void Init(const int& a_width, const int& a_height, const std::string& a_title, const bool& a_fullscreen) override;
		virtual void SwapBuffers() override;

		GLFWwindow* GetWindow() { return m_window; }

	private:
		GLFWwindow* m_window;
	};
}