#include "Platform/OpenGL/OpenGLContext.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Events/Event.h"
#include "Events/ApplicationEvent.h"

#include "Log.h"

namespace Framework
{
	OpenGLContext::OpenGLContext(const int& a_width, const int& a_height, const std::string& a_title, const bool& a_fullscreen, Window::WindowData* a_windowData)
		: m_window(nullptr)
	{
		Init(a_width,a_height, a_title, a_fullscreen, nullptr, a_windowData);
	}

	OpenGLContext::~OpenGLContext()
	{
	}

	void OpenGLContext::Init(const int& a_width, const int& a_height, const std::string& a_title, const bool& a_fullscreen, void* a_window, Window::WindowData* a_windowData)
	{
		// Setup glfw,
		if (!glfwInit())
		{
			glfwSetErrorCallback([](int errorCode, const char* errorText)
			{
				EN_CORE_ERROR("GLFW Error ({0}, {1})", errorCode, errorText);
			});
		}
		m_window = glfwCreateWindow(a_width, a_height, a_title.c_str(), nullptr, nullptr);
		if (!m_window)
		{
		}
		glfwMakeContextCurrent(m_window);
		glfwSetWindowUserPointer(m_window, a_windowData);
		
		//Setup glad
		if (!gladLoadGL())
		{
		}
		
		glfwSetWindowSizeCallback(m_window, [](GLFWwindow* window, int width, int height)
		{
			Window::WindowData& data = *(Window::WindowData*)glfwGetWindowUserPointer(window);
		
			data.Width = width;
			data.Height = height;
		
			WindowResizeEvent event(width, height);
			data.EventCallback(event);
		});
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