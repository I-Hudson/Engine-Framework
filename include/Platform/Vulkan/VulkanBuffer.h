#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "Renderer/Buffer.h"
#include "Platform/Vulkan/Internal/VulkanCommand.h"
#include "Platform/Vulkan/Internal/VulkanQueue.h"

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
			VkDevice m_device;
			VkPhysicalDevice m_physicalDevice;
			VulkanCommand m_command;
			VulkanQueue m_queue;

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
			uint32_t m_ID;

			VkDevice m_device;
			VkPhysicalDevice m_physicalDevice;
			VulkanCommand m_command;
			VulkanQueue m_queue;

			VkBuffer m_id;
			VkDeviceMemory m_indexBufferMemory;
		};
	}
}