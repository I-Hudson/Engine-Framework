#include "Renderer/GraphicsContext.h"

#include "Renderer/Renderer.h"
#include "Platform/OpenGL/OpenGLContext.h"
#include "Platform/DirectX/DirectXContext.h"
#include "Platform/Vulkan/VulkanContext.h"

namespace Framework
{
	namespace Renderer
	{
		GraphicsContext* GraphicsContext::Create(const int& a_width, const int& a_height, const std::string& a_title,
			const bool& a_fullscreen, void* a_window, Window::WindowData* a_windowData)
		{
			switch (Renderer::GetAPI())
			{
			case RendererAPI::API::None: return nullptr;
			case RendererAPI::API::OpenGL: return new OpenGLContext(a_width, a_height, a_title, a_fullscreen, a_windowData);
			case RendererAPI::API::DirectX: return new DirectXContext(a_width, a_height, a_title, a_fullscreen, a_window);
			case RendererAPI::API::Vulkan: return new Vulkan::VulkanContext(a_width, a_height, a_title, a_fullscreen, a_windowData);
			}
			return nullptr;
		}
	}
}