#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <optional>
#include <cstdint>
#include <vector>

namespace Framework
{
	namespace Vulkan
	{
		/*
			Handle all the queues need for Vulkan.
		*/

		struct QueueFamilyIndices
		{
			std::optional<uint32_t> GraphicsFamily;
			std::optional<uint32_t> PresentFamily;

			bool IsComplete()
			{
				return GraphicsFamily.has_value() && PresentFamily.has_value();
			}
		};

		class VulkanQueue
		{
		public:
			VulkanQueue();
			~VulkanQueue();

			void SetContext(VulkanContext* vulkanContext) { m_vulkanContext = vulkanContext; }

			QueueFamilyIndices FindQueueFamilies(VkPhysicalDevice device);
			VkQueue* GetGraphicsQueue() { return &m_graphicsQueue; }
			VkQueue* GetPresentQueue() { return &m_presentQueue; }

		private:
			VulkanContext* m_vulkanContext;

			VkQueue m_graphicsQueue;
			VkQueue m_presentQueue;
		};
	}
}