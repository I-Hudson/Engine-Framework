#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "Renderer/Buffer.h"
#include "Platform/Vulkan/Internal/VulkanCommand.h"

namespace Framework
{
	namespace Vulkan
	{
		class VulkanVertexBuffer : public Renderer::VertexBuffer
		{
		public:
			VulkanVertexBuffer(float* aVertices, uint32_t aSize);
			VulkanVertexBuffer(Renderer::Vertex* aVertices, uint32_t aSize);
			virtual ~VulkanVertexBuffer();

			//Bind and unbind the buffer
			virtual void Bind(void* object = nullptr) const override;
			virtual void Unbind(void* object = nullptr) const override;

			//Set the internal layout of this buffer
			virtual void SetLayout(const Renderer::BufferLayout& aLayout) override;
			//Get the layout of this buffer 
			virtual const Renderer::BufferLayout& GetLayout() const override;
		

		private:
			void CreateBuffer(VkDevice device, VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory);
			void CopyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);
			uint32_t FindMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);

		private:
			VkDevice m_device;
			VulkanCommand m_command;

			VkBuffer m_id;
			VkDeviceMemory m_vertexBufferMemory;
		};

		class VulkanIndexBuffer : public Renderer::IndexBuffer
		{
		public:
			VulkanIndexBuffer(unsigned int* indices, unsigned int count);
			virtual ~VulkanIndexBuffer();

			virtual void SetSubData(const unsigned int* a_indices, const unsigned int& a_count) override;

			VkBuffer* GetBuffer() { return &m_id; }

			//Bind and unbind
			virtual void Bind() const override;
			virtual void Unbind() const override;

		private:
			void CreateBuffer(VkDevice device, VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory);
			void CopyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);
			uint32_t FindMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);

		private:
			uint32_t m_ID;

			VkDevice m_device;
			VulkanCommand m_command;

			VkBuffer m_id;
			VkDeviceMemory m_indexBufferMemory;
		};
	}
}