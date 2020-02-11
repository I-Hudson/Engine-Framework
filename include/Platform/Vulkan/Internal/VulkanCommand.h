#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <vector>

namespace Framework
{
	namespace Vulkan
	{
		class VulkanContext;

		class VulkanCommand
		{
		public:
			VulkanCommand();
			~VulkanCommand();

			void Setup(VulkanContext* context);
			void DestroyCommandPool();
			
			void CreateCommandBuffers(unsigned int count, std::vector<VkCommandBuffer>* buffers);
			void FreeCommandBuffers(std::vector<VkCommandBuffer>* buffers);

			void EndCommandRecord();

			VkCommandPool* GetCommandPool() { return &m_commandPool; }
			std::vector<VkCommandBuffer>* GetCommandBuffers() { return &m_commandBuffers; }

		private:
			VkCommandPool m_commandPool;

			std::vector<VkCommandBuffer> m_commandBuffers;
		};
	}
}