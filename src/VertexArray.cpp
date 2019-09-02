#include "VertexArray.h"
#include "Platform/OpenGL/OpenGLVertexArray.h"
#include "Renderer.h"

namespace Framework
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