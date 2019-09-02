#include "RenderCommand.h"

#include "Platform/OpenGL/OpenGLRendererAPI.h"

namespace Framework
{
	RendererAPI* RenderCommand::sRendererAPI = new OpenGLRendererAPI;
}