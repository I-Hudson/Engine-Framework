#include "Application.h"

#include "Platform/OpenGL/OpenGLContext.h"
#include "Platform/DirectX/DirectXContext.h"
#include "Platform/Vulkan/VulkanContext.h"

#include "Platform/DirectX/DirectXShader.h"
#include "Platform/DirectX/DirectXBuffer.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stbi/stb_image.h>
#include <Events\ApplicationEvent.h>

namespace Framework
{
#define BIND_EVENT_FUNC(x) (std::bind(&Application::x, this, std::placeholders::_1))

	bool testVulkan = true;

	Application* Application::sInstance = nullptr;

	Application::Application()
		: m_isRunning(true), m_window(nullptr)
	{
		sInstance = this;
	}

	Application::~Application()
	{
	}

	bool Application::CreateApp(const int& a_width, const int& a_height, const char* a_title, const bool& a_runDemo)
	{
		Log::Init();
		EN_CORE_INFO("Core logger has been initialized");

		WindowProps props = WindowProps(a_title, a_width, a_height);
		m_window = Window::Create(props);
		m_window->SetEventCallback(BIND_EVENT_FUNC(OnEvent));

		Renderer::RenderCommand::Create();
		Renderer::RenderCommand::SetGraphicsContext(m_window->GetGraphicsContext());

		testVulkan = Renderer::RendererAPI::GetAPI() == Renderer::RendererAPI::API::Vulkan;

		if (!testVulkan)
		{
			OnImGuiCreate();
		}

		//Setup the main camera
		m_mainCamera = std::make_shared<Camera>();
		m_mainCamera->SetProjMatrix(45.0f, (float)a_width / (float)a_height, 0.1f, 1000.0f);

		if (testVulkan)
		{
			auto demoShader = m_shaderLibrary.Load("vulkanDemoShader", "./shaders/Vulkan/vert.spv", "./shaders/Vulkan/frag.spv");
		}

		//Create a demo cube and rotate
		if (a_runDemo)
		{
			auto demoShader = m_shaderLibrary.Load("demoShader", "./shaders/demoShader.glsl");
			//auto demoShader = m_shaderLibrary.Load("DirectX DemoShader", "./shaders/VertexShader.hlsl", "./shaders/PixelShader.hlsl");
			m_demoCube = std::make_shared<Cube>(1.0f);
		}

		Renderer::RenderCommand::SetDepthTest(true);
		Renderer::RenderCommand::SetCullFace(true);

		Renderer::RenderCommand::SetVSync(false);

		Renderer::Renderer::SetAmbiantLightColour({ 0.15f, 0.15f, 0.15f });
		Renderer::Renderer::SetAmbiantLightIntensity(0.05f);

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

		do
		{
			Time::UpdateTime();

			if (Renderer::RendererAPI::GetAPI() == Renderer::RendererAPI::API::DirectX)
			{
				//RenderCommand::SetClearColor(glm::vec4(0.0f, 0.0f, 1.0f, 1.0f));
				//const glm::vec4 eyePosition = glm::vec4(0, 0, -10, 1);
				//const glm::vec4 foucesPosition = glm::vec4(0, 0, 0, 1);
				//const glm::vec4 upVector = glm::vec4(0, 1, 0, 0);
				//
				//glm::mat4 viewMatrix = glm::lookAt(glm::vec3(eyePosition.xyz), glm::vec3(foucesPosition.xyz), glm::vec3(upVector.xyz));
				//
				//float aspectRaio = 1280 / 720;
				//glm::mat4 projectionMatrix = glm::perspective(glm::radians(90.0f), aspectRaio, 0.1f, 100.0f);
				//
				//
				//
				//auto context = std::dynamic_pointer_cast<DirectXContext>(m_window->GetGraphicsContext());
				//auto commandQueue = *context->m_commandQueue;
				//auto commandList = context->m_commandQueue->GetCommandList();
				//auto shader = std::dynamic_pointer_cast<DirectXShader>(m_shaderLibrary.GetShader(""));
				//
				//int currentBufferIndex = context->m_currentBackBufferIndex;
				//auto backBuffer = context->m_backBuffers[currentBufferIndex];
				//auto rtv = context->GetCurrentRenderTargetView();
				//auto dsv = shader->GetDescriptorHeap()->GetCPUDescriptorHandleForHeapStart();
				//
				//context->TransitionResource(commandList, backBuffer, D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET);
				//RenderCommand::Clear();
				//
				//commandList->SetPipelineState(shader->GetPipeLineState().Get());
				//commandList->SetGraphicsRootSignature(shader->GetRootSignature().Get());
				//// 
				//commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
				//commandList->IASetVertexBuffers(0, 1, std::dynamic_pointer_cast<DirectXVertexBuffer>(m_demoCube->GetVertexArray()->GetVertexBuffers()[0])->GetView());
				//commandList->IASetIndexBuffer(std::dynamic_pointer_cast<DirectXIndexBuffer>(m_demoCube->GetVertexArray()->GetIndexBuffer())->GetView());
				//
				//commandList->RSSetViewports(1, &D3D12_VIEWPORT(CD3DX12_VIEWPORT(0.0f, 0.0f, 1280.0f, 720.0f)));
				//commandList->RSSetScissorRects(1, &D3D12_RECT(CD3DX12_RECT(0, 0, 1280, 720)));
				////
				//commandList->OMSetRenderTargets(1, &rtv, false, &dsv);
				//
				//glm::mat4 mvp = glm::mat4(1.0f) * viewMatrix;
				//mvp *= projectionMatrix;
				//commandList->SetGraphicsRoot32BitConstants(0, sizeof(glm::mat4) / 4, &mvp, 0);
				//
				//int count = m_demoCube->GetVertexArray()->GetIndexBuffer()->GetCount();
				//commandList->DrawIndexedInstanced(36, 1, 0, 0, 0);
				//
				//
				////Present
				//{
				//	CD3DX12_RESOURCE_BARRIER barrier = CD3DX12_RESOURCE_BARRIER::Transition(backBuffer.Get(),
				//		D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT);
				//
				//	commandList->ResourceBarrier(1, &barrier);
				//
				//	context->m_frameFenceValues[currentBufferIndex] = commandQueue.ExecuteCommandList(commandList);
				//
				//	currentBufferIndex = context->Present();
				//
				//	commandQueue.WaitForFenceValue(context->m_frameFenceValues[currentBufferIndex]);
				//}
			}
			else
			{
				GLFWwindow* window = static_cast<GLFWwindow*>(m_window->GetGraphicsContext()->GetWindow());
				m_isRunning = !glfwWindowShouldClose(window);

				if (!testVulkan)
				{
					ImGui_ImplOpenGL3_NewFrame();
					ImGui_ImplGlfw_NewFrame();
					ImGui::NewFrame();


					m_mainCamera->Update(Time::GetDeltaTime());
				}

				Update();

				m_guiManager.OnUpdate();

				for (auto layer : m_layerStack)
				{
					layer->OnUpdate();
				}

				Renderer::RenderCommand::Clear();

				Renderer::Vertex vertices[3] =
				{
					Renderer::Vertex({0.0f, -0.5f, 0.0f, 0.0f}, {0.0f, 1.0f, 1.0f, 1.0f}, {0.0f, 0.0f, 0.0f, 0.0f}, {0.0f, 0.0f}),
					Renderer::Vertex({0.5f, 0.5f, 0.0f, 0.0f}, {1.0f, 0.0f, 1.0f, 1.0f}, {0.0f, 0.0f, 0.0f, 0.0f}, {0.0f, 0.0f}),
					Renderer::Vertex({-0.5f, 0.5f, 0.0f, 0.0f}, {1.0f, 1.0f, 0.0f, 1.0f}, {0.0f, 0.0f, 0.0f, 0.0f}, {0.0f, 0.0f})
				};

				std::shared_ptr<Renderer::VertexBuffer> vb;
				vb.reset(Renderer::VertexBuffer::Create(vertices, 3));

				unsigned int indices[] =
				{
					0,1,2
				};

				std::shared_ptr<Renderer::IndexBuffer> ib;
				ib.reset(Renderer::IndexBuffer::Create(indices, 3));

				std::shared_ptr<Renderer::VertexArray> va;
				va.reset(Renderer::VertexArray::Create());
				va->AddVertexBuffer(vb);
				va->AddIndexBuffer(ib);

				Renderer::Renderer::Begin(*m_mainCamera);
				if (a_runDemo)
				{
					auto shader = m_shaderLibrary.GetShader("demoShader");
					//m_demoCube->Rotate(3.5f * Time::GetDeltaTime(), glm::vec3(0, 1, 0));
					//m_demoCube->Rotate(3.5f * Time::GetDeltaTime(), glm::vec3(1, 1, 0));
					Renderer::Renderer::Submit(shader, m_demoCube->GetVertexArray(), m_demoCube->GetTransform());
				}
				Draw();

				if (testVulkan)
				{
					Renderer::Renderer::Submit(m_shaderLibrary.GetShader("vulkanDemoShader"), va);
				}

				//Renderer::SubmitBatched(shader);
				Renderer::Renderer::EndScene();

				if (!testVulkan)
				{
					ImGui::Render();
					ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
				}
			}
			//GLFW
			m_window->OnUpdate();

		} while (m_isRunning);

		DestroyApp();
	}

	void Application::DestroyApp()
	{
		Destroy();

		if (!testVulkan)
		{
			OnImGuiDestroy();
		}

		m_guiManager.Destroy();
		m_layerStack.Destroy();
		m_textureLibrary.ReleaseAll();
		m_shaderLibrary.ReleaseAll();

		Renderer::Renderer::Destroy();

		Renderer::RenderCommand::Destroy();

		m_window->Destroy();
		delete m_window;

		Log::Destroy();
	}

	void Application::PushLayer(Layer* aLayer)
	{
		m_layerStack.PushLayer(aLayer);
		aLayer->OnAttach();
	}

	void Application::PushOverlay(Layer* aLayer)
	{
		m_layerStack.PushOverlay(aLayer);
		aLayer->OnAttach();
	}

	void Application::OnEvent(Event& a_event)
	{
		EventDispatcher dispatcher(a_event);

		EN_CORE_INFO("{0}", a_event.ToString());

		m_shaderLibrary.OnEvent(a_event);

		for (auto layer : m_layerStack)
		{
			layer->OnEvent(a_event);
			if (a_event.Handled)
			{
				EN_CORE_INFO("Event, {0} was handled by {1}", a_event.ToString(), layer->GetName());
				break;
			}
		}
	}

	void Application::OnImGuiCreate()
	{
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO();

		GLFWwindow* w = static_cast<GLFWwindow*>(GetWindow()->GetGraphicsContext()->GetWindow());
		ImGui_ImplGlfw_InitForOpenGL(w, true);
		ImGui_ImplOpenGL3_Init("#version 400");

		ImGui::StyleColorsClassic();
	}

	void Application::OnImGuiDestroy()
	{
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
	}
}