#include "Renderer/RenderCommand.h"

#include "Platform/OpenGL/OpenGLRendererAPI.h"
#include "Platform/DirectX/DirectXRendererAPI.h"
#include "Platform/Vulkan/VulkanRendererAPI.h"

namespace Framework
{
	namespace Renderer
	{
		RendererAPI* RenderCommand::sRendererAPI = new OpenGLRendererAPI;
	}
}