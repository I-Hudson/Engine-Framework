#include "Platform/OpenGL/OpenGLContext.h"

#include <glad/glad.h>
#include "GLFW/glfw3.h"

#include "Log.h"

namespace Framework
{
	OpenGLContext::OpenGLContext(const int& a_width, const int& a_height, const std::string& a_title, const bool& a_fullscreen)
	{
		Init(a_width,a_height, a_title, a_fullscreen, nullptr);
	}

	OpenGLContext::~OpenGLContext()
	{
		glfwDestroyWindow(m_window);
	}

	void OpenGLContext::Init(const int& a_width, const int& a_height, const std::string& a_title, const bool& a_fullscreen, void* a_window)
	{
		// Setup glfw,
		if (!glfwInit())
		{
		}
		m_window = glfwCreateWindow(a_width, a_height, a_title.c_str(), nullptr, nullptr);
		if (!m_window)
		{
		}
		glfwMakeContextCurrent(m_window);
		glfwSetWindowUserPointer(m_window, this);

		//Setup glad
		if (!gladLoadGL())
		{
		}
	}

	void OpenGLContext::Destroy()
	{
		glfwDestroyWindow(m_window);
		glfwTerminate();
	}

	void OpenGLContext::SwapBuffers()
	{
		glfwSwapBuffers(m_window);
		glfwPollEvents();
	}
}