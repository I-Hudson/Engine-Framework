#include "Buffer.h"
#include "Renderer.h"

#include "Platform/OpenGL/OpenGLBuffer.h"

namespace Framework
{
	VertexBuffer* VertexBuffer::Create(float* aVertices, uint32_t aSize)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:
			//EN_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
			return nullptr;


		case RendererAPI::API::OpenGL:
			return new OpenGLVertexBuffer(aVertices, aSize);
		}
		//EN_CORE_ASSERT(false, "Unknow RendererAPI!");
		return nullptr;
	}

	VertexBuffer * VertexBuffer::Create(Vertex * aVertices, uint32_t aSize)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:
			//EN_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
			return nullptr;


		case RendererAPI::API::OpenGL:
			return new OpenGLVertexBuffer(aVertices, aSize);
		}
		//EN_CORE_ASSERT(false, "Unknow RendererAPI!");
		return nullptr;
	}

	IndexBuffer* IndexBuffer::Create(uint32_t* aIndices, uint32_t aSize)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:
			//EN_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
			return nullptr;


		case RendererAPI::API::OpenGL:
			return new OpenGLIndexBuffer(aIndices, aSize);
		}
		//EN_CORE_ASSERT(false, "Unknow RendererAPI!");
		return nullptr;
	}
}