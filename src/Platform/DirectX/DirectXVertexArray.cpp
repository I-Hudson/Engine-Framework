#include "Platform\DirectX\DirectXVertexArray.h"

namespace Framework
{
	DirectXVertexArray::DirectXVertexArray()
	{
	}

	DirectXVertexArray::~DirectXVertexArray()
	{
	}

	void DirectXVertexArray::Bind(void* object) const
	{
	}

	void DirectXVertexArray::Unbind(void* object) const
	{
	}

	void DirectXVertexArray::AddVertexBuffer(const std::shared_ptr<Renderer::VertexBuffer>& a_vertexBuffer)
	{
		m_vertexBuffers.push_back(a_vertexBuffer);
	}

	void DirectXVertexArray::AddIndexBuffer(const std::shared_ptr<Renderer::IndexBuffer>& a_indexBuffer)
	{
		m_indexBuffer = a_indexBuffer;
	}

	const std::vector<std::shared_ptr<Renderer::VertexBuffer>>& DirectXVertexArray::GetVertexBuffers() const
	{
		return m_vertexBuffers;
	}

	const std::shared_ptr<Renderer::IndexBuffer>& DirectXVertexArray::GetIndexBuffer() const
	{
		return m_indexBuffer;
	}
}