#include "Application.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Platform/OpenGL/OpenGLContext.h"
#include "Platform/DirectX/DirectXContext.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stbi/stb_image.h"

namespace Framework
{
	Application* Application::sInstance = nullptr;

	Application::Application()
		: m_isRunning(true)
	{
		sInstance = this;
	}

	Application::~Application()
	{
	}

	bool Application::CreateApp(const int& a_width, const int& a_height, const char* a_title, const bool& a_runDemo,
									HINSTANCE* a_hInstance, HINSTANCE* a_hPrevInstance, LPSTR* a_lpCmdLine, int a_nCmdShow)
	{
		m_context = GraphicsContext::Create(a_width, a_height, a_title);

		if (Renderer::GetAPI() == RendererAPI::API::DirectX)
		{
			auto context = std::dynamic_pointer_cast<DirectXContext>(m_context);
			context->PostInit(a_width, a_height, a_title, false, *a_hInstance, *a_hPrevInstance, *a_lpCmdLine, a_nCmdShow);
		}

		Log::Init();
		EN_CORE_INFO("Core logger has been initialized");

		//Setup the main camera
		m_mainCamera = std::make_shared<Camera>();
		m_mainCamera->SetProjMatrix(45.0f, (float)a_width / (float)a_height, 0.1f, 1000.0f);

		//Create a demo cube and rotate
		if (a_runDemo)
		{
			auto demoShader = m_shaderLibrary.Load("./shaders/demoShader.glsl");
			m_demoCube = std::make_shared<Cube>(1.0f);
			m_demoCube->Translate(glm::vec3(0, 0, 0));
		}

		//glEnable(GL_CULL_FACE);
		//glCullFace(GL_BACK);

		if (!OnCreate())
		{
			DestroyApp();

			return false;
		}
		return true;
	}

	void Application::RunApp(const int& a_width, const int& a_height, const char* a_title, const bool& a_runDemo,
							HINSTANCE* a_hInstance, HINSTANCE* a_hPrevInstance, LPSTR* a_lpCmdLine, int a_nCmdShow)
	{
		if (!CreateApp(a_width, a_height, a_title, a_runDemo, a_hInstance, a_hPrevInstance, a_lpCmdLine, a_nCmdShow))
		{
			DestroyApp();
			return;
		}

		//RenderCommand::SetClearColor({ 0.0f, 0.0f, 0.0f, 1.0f });
		do
		{
			Time::UpdateTime();

			if (RendererAPI::GetAPI() == RendererAPI::API::DirectX)
			{
				RenderCommand::SetClearColor(glm::vec4(0.0f, 0.0f, 1.0f, 1.0f));
			}
			else
			{

				m_isRunning = !glfwWindowShouldClose(std::dynamic_pointer_cast<OpenGLContext>(m_context)->GetWindow());

				m_mainCamera->Update(Time::GetDeltaTime());
				Update();

				RenderCommand::Clear();

				Renderer::Begin(*m_mainCamera);
				if (a_runDemo)
				{
					//m_demoCube->Rotate(3.5f * Time::GetDeltaTime(), glm::vec3(0, 1, 0));
					//m_demoCube->Rotate(3.5f * Time::GetDeltaTime(), glm::vec3(1, 1, 0));
					//Renderer::Submit(m_shaderLibrary.GetShader("demoShader"), m_demoCube->GetVertexArray(), m_demoCube->GetTransform());
				}
				Draw();
				Renderer::EndScene();
			}
			//GLFW
			m_context->SwapBuffers();

		} while (m_isRunning);

		DestroyApp();
	}

	void Application::DestroyApp()
	{
		Destroy();
		m_textureLibrary.ReleaseAll();
		m_shaderLibrary.ReleaseAll();

		glfwTerminate();
	}
}