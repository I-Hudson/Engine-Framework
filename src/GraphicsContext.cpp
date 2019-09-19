#include "GraphicsContext.h"

#include "Renderer.h"
#include "Platform/OpenGL/OpenGLContext.h"

namespace Framework
{
	std::shared_ptr<GraphicsContext> GraphicsContext::Create(const int & a_width, const int & a_height, const std::string & a_title, const bool & a_fullscreen)
	{
		switch (Renderer::GetAPI())
		{
			case RendererAPI::API::None: return nullptr;
			case RendererAPI::API::OpenGL: return std::make_shared<OpenGLContext>(a_width, a_height, a_title, a_fullscreen);
		}
		return nullptr;
	}
}