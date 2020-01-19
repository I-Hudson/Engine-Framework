#include "Renderer/VertexArray.h"
#include "Renderer/Renderer.h"

#include "Platform/OpenGL/OpenGLVertexArray.h"

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
			}

			return nullptr;
		}
	}
}