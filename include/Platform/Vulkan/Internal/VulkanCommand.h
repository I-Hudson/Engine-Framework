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
			
			void CreateCommandBuffers();
			void FreeCommandBuffers();

			VkCommandPool* GetCommandPool() { return &m_commandPool; }
			std::vector<VkCommandBuffer>* GetCommandBuffers() { return &m_commandBuffers; }

		private:
			VulkanContext* m_vkContext;

			VkCommandPool m_commandPool;

			std::vector<VkCommandBuffer> m_commandBuffers;
		};
	}
}