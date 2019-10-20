#include "RenderCommand.h"

#include "Platform/OpenGL/OpenGLRendererAPI.h"
#include "Platform/DirectX/DirectXRendererAPI.h"

namespace Framework
{
	RendererAPI* RenderCommand::sRendererAPI = new DirectXRendererAPI;
}