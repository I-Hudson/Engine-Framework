#include "Buffer.h"
#include "Renderer.h"

#include "Platform/OpenGL/OpenGLBuffer.h"

namespace Framework
{
	Vertex* VertexBuffer::GetVertex(const int& index)
	{
		if (index > m_count)
		{
			return nullptr;
		}
		return &m_vertics[index];
	}

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

	unsigned int IndexBuffer::GetIndice(const int& index)
	{
		if (index > m_count)
		{
			return 0;
		}
		return m_indices[index];
	}

	IndexBuffer* IndexBuffer::Create(unsigned int* aIndices, unsigned int aSize)
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