#include "Platform/Vulkan/VulkanVertexArray.h"
#include "Platform/Vulkan/VulkanBuffer.h"

#include <iostream>
#include <memory>

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

namespace Framework
{
	namespace Vulkan
	{
		VulkanVertexArray::VulkanVertexArray()
		{
		}

		VulkanVertexArray::~VulkanVertexArray()
		{
		}

		void VulkanVertexArray::Destroy()
		{
			for (size_t i = 0; i < m_vertexBuffers.size(); ++i)
			{
				m_vertexBuffers[i]->Destroy();
			}

			m_indexBuffer->Destroy();
		}

		void VulkanVertexArray::Bind(void* object) const
		{
			if (object == nullptr)
			{
				return;
			}


			for (auto vb : m_vertexBuffers)
			{
				vb->Bind(object);
			}

			VkBuffer indexBuffer = *static_cast<Vulkan::VulkanIndexBuffer*>(&*m_indexBuffer)->GetBuffer();
			vkCmdBindIndexBuffer(static_cast<VkCommandBuffer>(object), indexBuffer, 0, VK_INDEX_TYPE_UINT32);
		}

		void VulkanVertexArray::Unbind(void* object) const
		{
			for (auto vb : m_vertexBuffers)
			{
				vb->Unbind(object);
			}
		}

		void VulkanVertexArray::SetSubVertexData(const Renderer::Vertex* a_vertices, const unsigned int& a_bufferSize)
		{
		}

		void VulkanVertexArray::SetSubIndexData(const unsigned int* a_indices, const unsigned int& a_count)
		{
		}

		void VulkanVertexArray::AddVertexBuffer(const std::shared_ptr<Renderer::VertexBuffer>& a_vertexBuffer)
		{
			m_vertexBuffers.push_back(a_vertexBuffer);
		}

		void VulkanVertexArray::AddIndexBuffer(const std::shared_ptr<Renderer::IndexBuffer>& a_indexBuffer)
		{
			m_indexBuffer = a_indexBuffer;
		}

		const std::vector<std::shared_ptr<Renderer::VertexBuffer>>& VulkanVertexArray::GetVertexBuffers() const
		{
			// TODO: insert return statement here
			return m_vertexBuffers;
		}

		const std::shared_ptr<Renderer::IndexBuffer>& VulkanVertexArray::GetIndexBuffer() const
		{
			// TODO: insert return statement here
			return m_indexBuffer;
		}
	}
}
