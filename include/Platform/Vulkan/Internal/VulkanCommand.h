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

			static void CreateCommandPool(VkCommandPool* commandPool);
			static void FreeCommandPool(VkCommandPool* commandPool);
			
			static void CreateCommandBuffers(unsigned int count, std::vector<VkCommandBuffer>* buffers, VkCommandPool* commandPool);
			static void FreeCommandBuffers(std::vector<VkCommandBuffer>* buffers, VkCommandPool* commandPool);

			static void EndCommandRecord(std::vector<VkCommandBuffer>* buffers);
		};
	}
}