#include "..\include\Application.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace Framework
{
	Application* Application::sInstance = nullptr;

	Application::Application()
		: m_isRunning(true), m_window(nullptr)
	{
	}

	Application::~Application()
	{
	}

	bool Application::CreateApp(const int& a_width, const int& a_height, const char* a_title, const bool& a_runDemo)
	{
		// Setup glfw,
		if (!glfwInit())
		{
			//ERRSTR("GLFW failed to initialize");
			DestroyApp();
			return false;
		}
		m_window = glfwCreateWindow(a_width, a_height, a_title, nullptr, nullptr);
		if (!m_window)
		{
			//ERRSTR("GLFW unable to create window");
			DestroyApp();
			return false;
		}
		glfwMakeContextCurrent(m_window);
		glfwSetWindowUserPointer(m_window, this);

		//Setup glad
		if (!gladLoadGL())
		{
			//ERRSTR("GLAD failed to initialize");
			DestroyApp();
			return false;
		}

		//Setup the main camera
		m_mainCamera = std::make_shared<Camera>();
		m_mainCamera->SetProjMatrix(45.0f, (float)a_width / (float)a_height, 0.1f, 1000.0f);

		//Create a demo cube and rotate
		if (a_runDemo)
		{
			m_demoShader = std::make_shared<Shader>("./shaders/demoShader.glsl");
			m_demoCube = std::make_shared<Cube>(1.0f);
			m_demoCube->Translate(glm::vec3(0, 0, 0));
		}

		if (!OnCreate())
		{
			DestroyApp();

			return false;
		}
		return true;
	}

	void Application::RunApp(const int& a_width, const int& a_height, const char* a_title, const bool& a_runDemo)
	{
		if (!CreateApp(a_width, a_height, a_title, a_runDemo))
		{
			DestroyApp();
			return;
		}

		RenderCommand::SetClearColor({ 0.0f, 0.0f, 0.0f, 1.0f });
		do
		{
			Time::UpdateTime();
			m_isRunning = !glfwWindowShouldClose(m_window);

			m_mainCamera->Update(Time::GetDeltaTime());
			Update();

			RenderCommand::Clear();

			Renderer::Begin(*m_mainCamera);
			if (a_runDemo)
			{
				//m_demoCube->Rotate(3.5f * Time::GetDeltaTime(), glm::vec3(0, 1, 0));
				//m_demoCube->Rotate(3.5f * Time::GetDeltaTime(), glm::vec3(1, 1, 0));
				Renderer::Submit(m_demoShader, m_demoCube->GetVertexArray(), m_demoCube->GetTransform());
			}
			Draw();
			Renderer::EndScene();

			//GLFW
			glfwSwapBuffers(m_window);
			glfwPollEvents();
		} while (m_isRunning);

		DestroyApp();
	}

	void Application::DestroyApp()
	{
		Destroy();
		glfwTerminate();

		if (m_window)
		{
			glfwDestroyWindow(m_window);
		}
	}
}