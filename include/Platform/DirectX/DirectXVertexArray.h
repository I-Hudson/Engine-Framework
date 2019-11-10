#pragma once

#include "VertexArray.h"

namespace Framework
{
	class DirectXVertexArray : public VertexArray
	{
	public:
		DirectXVertexArray();
		virtual ~DirectXVertexArray();

		//Bind and unbind
		virtual void Bind() const override;
		virtual void Unbind() const override;

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