#include "Platform/Vulkan/VulkanBuffer.h"
#include "Platform/Vulkan/VulkanContext.h"
#include "Platform/Vulkan/VulkanUtils.h"

#include "Application.h"

namespace Framework
{
	namespace Vulkan
	{
		VulkanVertexBuffer::VulkanVertexBuffer(float* aVertices, uint32_t aSize)
		{
			m_device = *static_cast<Vulkan::VulkanContext*>(Application::Get().GetWindow()->GetGraphicsContext())->GetVulkanDevice()->GetDevice();
			m_physicalDevice = *static_cast<Vulkan::VulkanContext*>(Application::Get().GetWindow()->GetGraphicsContext())->GetVulkanDevice()->GetPhyiscalDevice();
			m_command = *static_cast<Vulkan::VulkanContext*>(Application::Get().GetWindow()->GetGraphicsContext())->GetVulkanCommand();
			m_queue = *static_cast<Vulkan::VulkanContext*>(Application::Get().GetWindow()->GetGraphicsContext())->GetVulkanQueue();

			int bufferSize = aSize;

			m_bufferSize = bufferSize;

			// RAM Buffer
			VkBuffer stagingBuffer;
			VkDeviceMemory stagingBufferMemory;
			VulkanUtils::CreateBuffer(m_device, m_physicalDevice, bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
				stagingBuffer, stagingBufferMemory);

			// Set the buffer in the ram to the vertices passed through.
			void* data;
			vkMapMemory(m_device, stagingBufferMemory, 0, bufferSize, 0, &data);
			memcpy(data, aVertices, (size_t)bufferSize);
			vkUnmapMemory(m_device, stagingBufferMemory);

			// VRAM buffer
			VulkanUtils::CreateBuffer(m_device, m_physicalDevice, bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
				VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, m_id, m_vertexBufferMemory);

			// Copy ram buffer to GPU.
			VulkanUtils::CopyBuffer(m_device, *m_command.GetCommandPool(), *m_queue.GetGraphicsQueue(), stagingBuffer, m_id, bufferSize);

			vkDestroyBuffer(m_device, stagingBuffer, nullptr);
			vkFreeMemory(m_device, stagingBufferMemory, nullptr);
		}

		VulkanVertexBuffer::VulkanVertexBuffer(Renderer::Vertex* aVertices, uint32_t aSize)
		{
			m_device = *static_cast<Vulkan::VulkanContext*>(Application::Get().GetWindow()->GetGraphicsContext())->GetVulkanDevice()->GetDevice();
			m_physicalDevice = *static_cast<Vulkan::VulkanContext*>(Application::Get().GetWindow()->GetGraphicsContext())->GetVulkanDevice()->GetPhyiscalDevice();
			m_command = *static_cast<Vulkan::VulkanContext*>(Application::Get().GetWindow()->GetGraphicsContext())->GetVulkanCommand();
			m_queue = *static_cast<Vulkan::VulkanContext*>(Application::Get().GetWindow()->GetGraphicsContext())->GetVulkanQueue();

			int bufferSize = aSize;

			m_bufferSize = bufferSize;

			// RAM Buffer
			VkBuffer stagingBuffer;
			VkDeviceMemory stagingBufferMemory;
			VulkanUtils::CreateBuffer(m_device, m_physicalDevice, bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
				stagingBuffer, stagingBufferMemory);

			// Set the buffer in the ram to the vertices passed through.
			void* data;
			vkMapMemory(m_device, stagingBufferMemory, 0, bufferSize, 0, &data);
			memcpy(data, aVertices, (size_t)bufferSize);
			vkUnmapMemory(m_device, stagingBufferMemory);

			// VRAM buffer
			VulkanUtils::CreateBuffer(m_device, m_physicalDevice, bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
				VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, m_id, m_vertexBufferMemory);

			// Copy ram buffer to GPU.
			VulkanUtils::CopyBuffer(m_device, *m_command.GetCommandPool(), *m_queue.GetGraphicsQueue(), stagingBuffer, m_id, bufferSize);

			vkDestroyBuffer(m_device, stagingBuffer, nullptr);
			vkFreeMemory(m_device, stagingBufferMemory, nullptr);
		}

		VulkanVertexBuffer::~VulkanVertexBuffer()
		{
			vkDestroyBuffer(m_device, m_id, nullptr);
			vkFreeMemory(m_device, m_vertexBufferMemory, nullptr);
		}

		void VulkanVertexBuffer::Bind(void* object) const
		{
			if (object == nullptr)
			{
				return;
			}

			VkBuffer vertexBuffers[] = { m_id };
			VkDeviceSize offsets[] = { 0 };
			vkCmdBindVertexBuffers(static_cast<VkCommandBuffer>(object), 0, 1, vertexBuffers, offsets);
		}

		void VulkanVertexBuffer::Unbind(void* object) const
		{
		}

		void VulkanVertexBuffer::SetLayout(const Renderer::BufferLayout& aLayout)
		{
		}

		const Renderer::BufferLayout& VulkanVertexBuffer::GetLayout() const
		{
			// TODO: insert return statement here
			return Renderer::BufferLayout();
		}


		VulkanIndexBuffer::VulkanIndexBuffer(unsigned int* indices, unsigned int count)
		{
			m_device = *static_cast<Vulkan::VulkanContext*>(Application::Get().GetWindow()->GetGraphicsContext())->GetVulkanDevice()->GetDevice();
			m_physicalDevice = *static_cast<Vulkan::VulkanContext*>(Application::Get().GetWindow()->GetGraphicsContext())->GetVulkanDevice()->GetPhyiscalDevice();
			m_command = *static_cast<Vulkan::VulkanContext*>(Application::Get().GetWindow()->GetGraphicsContext())->GetVulkanCommand();
			m_queue = *static_cast<Vulkan::VulkanContext*>(Application::Get().GetWindow()->GetGraphicsContext())->GetVulkanQueue();

			m_count = count;

			VkDeviceSize bufferSize = sizeof(indices[0]) * count;

			VkBuffer stagingBuffer;
			VkDeviceMemory stagingBufferMemory;
			VulkanUtils::CreateBuffer(m_device, m_physicalDevice, bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);

			void* data;
			vkMapMemory(m_device, stagingBufferMemory, 0, bufferSize, 0, &data);
			memcpy(data, indices, (size_t)bufferSize);
			vkUnmapMemory(m_device, stagingBufferMemory);

			VulkanUtils::CreateBuffer(m_device, m_physicalDevice, bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, m_id, m_indexBufferMemory);

			VulkanUtils::CopyBuffer(m_device, *m_command.GetCommandPool(), *m_queue.GetGraphicsQueue(), stagingBuffer, m_id, bufferSize);

			vkDestroyBuffer(m_device, stagingBuffer, nullptr);
			vkFreeMemory(m_device, stagingBufferMemory, nullptr);
		}

		VulkanIndexBuffer::~VulkanIndexBuffer()
		{
			vkDestroyBuffer(m_device, m_id, nullptr);
			vkFreeMemory(m_device, m_indexBufferMemory, nullptr);
		}

		void VulkanIndexBuffer::SetSubData(const unsigned int* a_indices, const unsigned int& a_count)
		{
		}

		void VulkanIndexBuffer::Bind() const
		{
		}

		void VulkanIndexBuffer::Unbind() const
		{
		}
	}
}