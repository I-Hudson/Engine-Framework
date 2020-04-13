#include "Renderer/GBuffer.h"

#include "Renderer/RendererAPI.h"

#include "Platform/OpenGL/OpenGLGBuffer.h"
#include "Platform/Vulkan/VulkanGBuffer.h"

namespace Framework
{
	namespace Renderer
	{
		GBuffer* GBuffer::Create()
		{
			GBuffer* returnBuffer = nullptr;

			switch (RendererAPI::GetAPI())
			{
			case RendererAPI::API::None: break;
			case RendererAPI::API::OpenGL: returnBuffer = new OpenGLGBuffer(); break;
			case RendererAPI::API::Vulkan: returnBuffer = new Vulkan::VulkanGBuffer(); break;
			case RendererAPI::API::DirectX: returnBuffer = nullptr; break;
			}

			returnBuffer->Init();
			return returnBuffer;
		}
	}
}