#include "Platform/Vulkan/VulkanBuffer.h"
#include "Platform/Vulkan/VulkanContext.h"

#include "Application.h"

namespace Framework
{
	namespace Vulkan
	{
		VulkanVertexBuffer::VulkanVertexBuffer(float* aVertices, uint32_t aSize)
		{
			m_device = *static_cast<Vulkan::VulkanContext*>(Application::Get().GetWindow()->GetGraphicsContext())->GetVulkanDevice()->GetDevice();
			m_command = *static_cast<Vulkan::VulkanContext*>(Application::Get().GetWindow()->GetGraphicsContext())->GetVulkanCommand();

			int bufferSize = sizeof(aVertices[0]) * aSize;

			m_bufferSize = bufferSize;

			VkBuffer stagingBuffer;
			VkDeviceMemory stagingBufferMemory;
			CreateBuffer(m_device, bufferSize, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
				stagingBuffer, stagingBufferMemory);

			void* data;
			vkMapMemory(m_device, stagingBufferMemory, 0, bufferSize, 0, &data);
			memcpy(data, aVertices, (size_t)bufferSize);
			vkUnmapMemory(m_device, stagingBufferMemory);

			CreateBuffer(m_device, bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
				VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, m_id, m_vertexBufferMemory);

			CopyBuffer(stagingBuffer, m_id, bufferSize);

			vkDestroyBuffer(m_device, stagingBuffer, nullptr);
			vkFreeMemory(m_device, stagingBufferMemory, nullptr);
		}

		VulkanVertexBuffer::VulkanVertexBuffer(Renderer::Vertex* aVertices, uint32_t aSize)
		{
			m_device = *static_cast<Vulkan::VulkanContext*>(Application::Get().GetWindow()->GetGraphicsContext())->GetVulkanDevice()->GetDevice();
			m_command = *static_cast<Vulkan::VulkanContext*>(Application::Get().GetWindow()->GetGraphicsContext())->GetVulkanCommand();

			int bufferSize = sizeof(aVertices[0]) * aSize;

			m_bufferSize = bufferSize;

			// RAM Buffer
			VkBuffer stagingBuffer;
			VkDeviceMemory stagingBufferMemory;
			CreateBuffer(m_device, bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
				stagingBuffer, stagingBufferMemory);

			// Set the buffer in the ram to the vertices passed through.
			void* data;
			vkMapMemory(m_device, stagingBufferMemory, 0, bufferSize, 0, &data);
			memcpy(data, aVertices, (size_t)bufferSize);
			vkUnmapMemory(m_device, stagingBufferMemory);

			// VRAM buffer
			CreateBuffer(m_device, bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
				VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, m_id, m_vertexBufferMemory);

			// Copy ram buffer to GPU.
			CopyBuffer(stagingBuffer, m_id, bufferSize);

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

		void VulkanVertexBuffer::CreateBuffer(VkDevice device, VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory)
		{
			VkBufferCreateInfo bufferInfo = {};
			bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
			bufferInfo.size = size;
			bufferInfo.usage = usage;
			bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

			if (vkCreateBuffer(device,
				&bufferInfo, nullptr, &buffer) != VK_SUCCESS)
			{
				EN_CORE_ERROR("Vulkan Vertex Buffer: Failed to create vertex buffer!");
			}

			VkMemoryRequirements memRequirements;
			vkGetBufferMemoryRequirements(device, buffer, &memRequirements);

			VkMemoryAllocateInfo allocInfo = {};
			allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
			allocInfo.allocationSize = memRequirements.size;
			allocInfo.memoryTypeIndex = FindMemoryType(memRequirements.memoryTypeBits, properties);
			if (vkAllocateMemory(device,
				&allocInfo, nullptr, &bufferMemory) != VK_SUCCESS)
			{
				EN_CORE_ERROR("failed to allocate vertex buffer memory!");
			}

			vkBindBufferMemory(device, buffer, bufferMemory, 0);
		}

		void VulkanVertexBuffer::CopyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size)
		{
			VkCommandBufferAllocateInfo allocInfo = {};
			allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
			allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
			allocInfo.commandPool = *m_command.GetCommandPool();
			allocInfo.commandBufferCount = 1;

			VkCommandBuffer commandBuffer;
			vkAllocateCommandBuffers(m_device, &allocInfo, &commandBuffer);

			VkCommandBufferBeginInfo beginInfo = {};
			beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
			beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

			vkBeginCommandBuffer(commandBuffer, &beginInfo);

			VkBufferCopy copyRegion = {};
			copyRegion.srcOffset = 0;
			copyRegion.dstOffset = 0;
			copyRegion.size = size;
			vkCmdCopyBuffer(commandBuffer, srcBuffer, dstBuffer, 1, &copyRegion);

			vkEndCommandBuffer(commandBuffer);

			VkSubmitInfo submitInfo = {};
			submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
			submitInfo.commandBufferCount = 1;
			submitInfo.pCommandBuffers = &commandBuffer;

			VkQueue queue = *static_cast<VulkanContext*>(Application::Get().GetWindow()->GetGraphicsContext())->GetVulkanQueue()->GetGraphicsQueue();
			vkQueueSubmit(queue, 1, &submitInfo, VK_NULL_HANDLE);
			vkQueueWaitIdle(queue);

			vkFreeCommandBuffers(m_device, *m_command.GetCommandPool(), 1, &commandBuffer);
		}

		uint32_t VulkanVertexBuffer::FindMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties)
		{
			VkPhysicalDeviceMemoryProperties memProperties;
			vkGetPhysicalDeviceMemoryProperties(*static_cast<Vulkan::VulkanContext*>(Application::Get().GetWindow()->GetGraphicsContext())->GetVulkanDevice()->GetPhyiscalDevice(), &memProperties);

			for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++)
			{
				if (typeFilter & (1 << i) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties)
				{
					return i;
				}
			}

			EN_CORE_ERROR("Vulkan Vertex Buffer: Failed to find suitable memory type!");

			return 0;
		}


		VulkanIndexBuffer::VulkanIndexBuffer(unsigned int* indices, unsigned int count)
		{
			m_device = *static_cast<Vulkan::VulkanContext*>(Application::Get().GetWindow()->GetGraphicsContext())->GetVulkanDevice()->GetDevice();
			m_command = *static_cast<Vulkan::VulkanContext*>(Application::Get().GetWindow()->GetGraphicsContext())->GetVulkanCommand();

			m_count = count;

			VkDeviceSize bufferSize = sizeof(indices[0]) * count;

			VkBuffer stagingBuffer;
			VkDeviceMemory stagingBufferMemory;
			CreateBuffer(m_device, bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);

			void* data;
			vkMapMemory(m_device, stagingBufferMemory, 0, bufferSize, 0, &data);
			memcpy(data, indices, (size_t)bufferSize);
			vkUnmapMemory(m_device, stagingBufferMemory);

			CreateBuffer(m_device, bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, m_id, m_indexBufferMemory);

			CopyBuffer(stagingBuffer, m_id, bufferSize);

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

		void VulkanIndexBuffer::CreateBuffer(VkDevice device, VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory)
		{
			VkBufferCreateInfo bufferInfo = {};
			bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
			bufferInfo.size = size;
			bufferInfo.usage = usage;
			bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

			if (vkCreateBuffer(device,
				&bufferInfo, nullptr, &buffer) != VK_SUCCESS)
			{
				EN_CORE_ERROR("Vulkan Vertex Buffer: Failed to create vertex buffer!");
			}

			VkMemoryRequirements memRequirements;
			vkGetBufferMemoryRequirements(device, buffer, &memRequirements);

			VkMemoryAllocateInfo allocInfo = {};
			allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
			allocInfo.allocationSize = memRequirements.size;
			allocInfo.memoryTypeIndex = FindMemoryType(memRequirements.memoryTypeBits, properties);
			if (vkAllocateMemory(device,
				&allocInfo, nullptr, &bufferMemory) != VK_SUCCESS)
			{
				EN_CORE_ERROR("failed to allocate vertex buffer memory!");
			}

			vkBindBufferMemory(device, buffer, bufferMemory, 0);
		}

		void VulkanIndexBuffer::CopyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size)
		{
			VkCommandBufferAllocateInfo allocInfo = {};
			allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
			allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
			allocInfo.commandPool = *m_command.GetCommandPool();
			allocInfo.commandBufferCount = 1;

			VkCommandBuffer commandBuffer;
			vkAllocateCommandBuffers(m_device, &allocInfo, &commandBuffer);

			VkCommandBufferBeginInfo beginInfo = {};
			beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
			beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

			vkBeginCommandBuffer(commandBuffer, &beginInfo);

			VkBufferCopy copyRegion = {};
			copyRegion.srcOffset = 0;
			copyRegion.dstOffset = 0;
			copyRegion.size = size;
			vkCmdCopyBuffer(commandBuffer, srcBuffer, dstBuffer, 1, &copyRegion);

			vkEndCommandBuffer(commandBuffer);

			VkSubmitInfo submitInfo = {};
			submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
			submitInfo.commandBufferCount = 1;
			submitInfo.pCommandBuffers = &commandBuffer;

			VkQueue queue = *static_cast<VulkanContext*>(Application::Get().GetWindow()->GetGraphicsContext())->GetVulkanQueue()->GetGraphicsQueue();
			vkQueueSubmit(queue, 1, &submitInfo, VK_NULL_HANDLE);
			vkQueueWaitIdle(queue);

			vkFreeCommandBuffers(m_device, *m_command.GetCommandPool(), 1, &commandBuffer);

		}

		uint32_t VulkanIndexBuffer::FindMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties)
		{
			VkPhysicalDeviceMemoryProperties memProperties;
			vkGetPhysicalDeviceMemoryProperties(*static_cast<Vulkan::VulkanContext*>(Application::Get().GetWindow()->GetGraphicsContext())->GetVulkanDevice()->GetPhyiscalDevice(), &memProperties);

			for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++)
			{
				if (typeFilter & (1 << i) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties)
				{
					return i;
				}
			}

			EN_CORE_ERROR("Vulkan Vertex Buffer: Failed to find suitable memory type!");

			return 0;
		}
	}
}