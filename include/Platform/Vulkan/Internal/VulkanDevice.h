#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <vector>

#include "Log.h"

namespace Framework
{
	namespace Vulkan
	{
		class VulkanContext;

		class VulkanDevice
		{
		public:
			VulkanDevice();
			~VulkanDevice();

			void Setup(VkInstance instance, VulkanContext* vulkanContext, std::vector<const char*> deviceExtensions);

			VkPhysicalDevice* GetPhyiscalDevice() { return &m_physicalDevice; }
			VkDevice* GetDevice() { return &m_device; }

			void Destroy();

		private:

			void PickPhysicalDevice(VkInstance instance, std::vector<const char*> deviceExtentions);
			bool IsDeviceSuitable(VkPhysicalDevice device, std::vector<const char*> deviceExtentions);
			bool CheckDeviceExtensionSupport(VkPhysicalDevice device, std::vector<const char*> deviceExtentions);
			int RateDeviceSuitability(VkPhysicalDevice device);
			void CreateLogicalDevice(std::vector<const char*> deviceExtentions);

		private:
			VulkanContext* m_vkContext;

			VkPhysicalDevice m_physicalDevice;
			VkDevice m_device;
		};
	}
}
