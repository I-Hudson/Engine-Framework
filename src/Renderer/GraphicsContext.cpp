#include "Renderer/GraphicsContext.h"

#include "Renderer/Renderer.h"
#include "Platform/OpenGL/OpenGLContext.h"
#include "Platform/DirectX/DirectXContext.h"
#include "Platform/Vulkan/VulkanContext.h"

namespace Framework
{
	namespace Renderer
	{
		GraphicsContext* GraphicsContext::s_instance = nullptr;

		GraphicsContext* GraphicsContext::Create(const int& a_width, const int& a_height, const std::string& a_title,
			const bool& a_fullscreen, void* a_window, Window::WindowData* a_windowData)
		{
			switch (Renderer::GetAPI())
			{
			case RendererAPI::API::None: return nullptr;
			case RendererAPI::API::OpenGL: s_instance = new OpenGLContext(a_width, a_height, a_title, a_fullscreen, a_windowData);
			case RendererAPI::API::DirectX: s_instance = new DirectXContext(a_width, a_height, a_title, a_fullscreen, a_window);
			case RendererAPI::API::Vulkan: s_instance = new Vulkan::VulkanContext(a_width, a_height, a_title, a_fullscreen, a_windowData);
			}
			return s_instance;
		}
	}
}