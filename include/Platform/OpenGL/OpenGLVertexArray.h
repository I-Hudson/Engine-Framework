#pragma once

#include "VertexArray.h"

namespace Framework
{
	class OpenGLVertexArray : public VertexArray
	{
	public:
		OpenGLVertexArray();
		virtual ~OpenGLVertexArray();

		//Bind and unbind
		virtual void Bind() const override;
		virtual void Unbind() const override;

		virtual void SetSubVertexData(const Vertex* a_vertices, const unsigned int& a_bufferSize) override;
		virtual void SetSubIndexData(const unsigned int* a_indices, const unsigned int& a_count) override;

		//Add a vertex buffer and index buffer
		virtual void AddVertexBuffer(const std::shared_ptr<VertexBuffer>& a_vertexBuffer) override;
		virtual void AddIndexBuffer(const std::shared_ptr<IndexBuffer>& a_indexBuffer) override;

		//Get the vertex buffers and index buffer
		virtual const std::vector<std::shared_ptr<VertexBuffer>>& GetVertexBuffers() const override;
		virtual const std::shared_ptr<IndexBuffer>& GetIndexBuffer() const override;

	private:
		uint32_t m_ID;
		std::vector<std::shared_ptr<VertexBuffer>> m_vertexBuffers;
		std::shared_ptr<IndexBuffer> m_indexBuffer;
	};
}