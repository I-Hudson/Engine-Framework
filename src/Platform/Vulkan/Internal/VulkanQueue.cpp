#include "Platform/Vulkan/VulkanContext.h"

namespace Framework
{
	namespace Vulkan
	{
		VulkanQueue::VulkanQueue()
		{
		}

		VulkanQueue::~VulkanQueue()
		{
		}

		QueueFamilyIndices VulkanQueue::FindQueueFamilies(VkPhysicalDevice device)
		{
			QueueFamilyIndices indices;

			uint32_t queueFamilyCount = 0;
			
			vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);

			std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
			vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

			for (size_t i = 0; i < queueFamilyCount; ++i)
			{
				if (queueFamilies[i].queueFlags & VK_QUEUE_GRAPHICS_BIT)
				{
					indices.GraphicsFamily = i;
				}

				VkBool32 presentSupport = false;
				vkGetPhysicalDeviceSurfaceSupportKHR(device, (uint32_t)i, *m_vulkanContext->GetVulkanSurface()->GetSurface(), &presentSupport);

				if (presentSupport)
				{
					indices.PresentFamily = i;
				}

				if (indices.IsComplete())
				{
					break;
				}
			}

			return indices;
		}
	}
}