#include "Renderer/VertexArray.h"
#include "Renderer/Renderer.h"

#include "Platform/OpenGL/OpenGLVertexArray.h"
#include "Platform/Vulkan/VulkanVertexArray.h"

namespace Framework
{
	namespace Renderer
	{
		VertexArray* VertexArray::Create()
		{
			switch (Renderer::GetAPI())
			{
			case RendererAPI::API::None:
				return nullptr;

			case RendererAPI::API::OpenGL:
				return new OpenGLVertexArray();

			case RendererAPI::API::Vulkan:
				return new Vulkan::VulkanVertexArray();
			}

			return nullptr;
		}
	}
}