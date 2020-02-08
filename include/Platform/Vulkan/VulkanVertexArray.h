#pragma once

#include "Renderer/VertexArray.h"

namespace Framework
{
	namespace Vulkan
	{
		class VulkanVertexArray : public Renderer::VertexArray
		{
		public:
			VulkanVertexArray();
			virtual ~VulkanVertexArray();

			virtual void Destroy() override;

			//Bind and unbind
			virtual void Bind(void* object) const override;
			virtual void Unbind(void* object) const override;

			virtual void SetSubVertexData(const Renderer::Vertex* a_vertices, const unsigned int& a_bufferSize) override;
			virtual void SetSubIndexData(const unsigned int* a_indices, const unsigned int& a_count) override;

			//Add a vertex buffer and index buffer
			virtual void AddVertexBuffer(const std::shared_ptr<Renderer::VertexBuffer>& a_vertexBuffer) override;
			virtual void AddIndexBuffer(const std::shared_ptr<Renderer::IndexBuffer>& a_indexBuffer) override;

			//Get the vertex buffers and index buffer
			virtual const std::vector<std::shared_ptr<Renderer::VertexBuffer>>& GetVertexBuffers() const override;
			virtual const std::shared_ptr<Renderer::IndexBuffer>& GetIndexBuffer() const override;

		private:
			uint32_t m_ID;
			std::vector<std::shared_ptr<Renderer::VertexBuffer>> m_vertexBuffers;
			std::shared_ptr<Renderer::IndexBuffer> m_indexBuffer;
		};
	}
}