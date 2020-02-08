#include "Renderer/RenderCommand.h"

#include "Platform/OpenGL/OpenGLRendererAPI.h"
#include "Platform/DirectX/DirectXRendererAPI.h"
#include "Platform/Vulkan/VulkanRendererAPI.h"

namespace Framework
{
	namespace Renderer
	{
		RendererAPI* RenderCommand::sRendererAPI = nullptr;

		void RenderCommand::Create()
		{
			switch (RendererAPI::GetAPI())
			{
			case RendererAPI::API::OpenGL:
				sRendererAPI = new OpenGLRendererAPI();
				break;
			case RendererAPI::API::Vulkan:
				sRendererAPI = new Vulkan::VulkanRendererAPI();
				break;
			case RendererAPI::API::DirectX:
				sRendererAPI = new DirectXRendererAPI();
				break;
			}
		}
	}
}