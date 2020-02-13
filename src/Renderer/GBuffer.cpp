#include "Renderer/GBuffer.h"

#include "Renderer/RendererAPI.h"

#include "Platform/Vulkan/VulkanGBuffer.h"

namespace Framework
{
	namespace Renderer
	{
		GBuffer* GBuffer::Create()
		{
			switch (RendererAPI::GetAPI())
			{
			case RendererAPI::API::None: return nullptr;
			case RendererAPI::API::OpenGL: return nullptr;
			case RendererAPI::API::Vulkan: return new Vulkan::VulkanGBuffer();
			case RendererAPI::API::DirectX: return nullptr;
			}
			return nullptr;
		}
	}
}