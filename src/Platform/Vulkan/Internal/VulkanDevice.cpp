#include "Platform/Vulkan/VulkanContext.h"

#include <map>
#include <set>

namespace Framework
{
	namespace Vulkan
	{
		VulkanDevice::VulkanDevice()
		{
		}

		VulkanDevice::~VulkanDevice()
		{
		}

		void VulkanDevice::Setup(VkInstance instance, VulkanContext* vulkanContext, std::vector<const char*> deviceExtentions)
		{
			m_vkContext = vulkanContext;
			PickPhysicalDevice(instance, deviceExtentions);
			CreateLogicalDevice(deviceExtentions);
		}

		void VulkanDevice::Destroy()
		{
			vkDestroyDevice(m_device, nullptr);
		}

		void VulkanDevice::PickPhysicalDevice(VkInstance instance, std::vector<const char*> deviceExtentions)
		{
			uint32_t deviceCount = 0;
			vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);
			if (deviceCount == 0)
			{
				EN_CORE_ERROR("Vulkan: Failed to find any GPUs with Vulkan support!");
			}

			std::vector<VkPhysicalDevice> devices(deviceCount);
			vkEnumeratePhysicalDevices(instance, &deviceCount, devices.data());

			std::multimap<int, VkPhysicalDevice> allDevices;

			for (auto device : devices)
			{
				int score = RateDeviceSuitability(device);
				allDevices.insert(std::make_pair(score, device));
			}

			if (allDevices.rbegin()->first > 0)
			{
				if (IsDeviceSuitable(allDevices.rbegin()->second, deviceExtentions))
				{
					m_physicalDevice = allDevices.rbegin()->second;
				}
			}
			else
			{
				EN_CORE_ERROR("Vulkan: Failed to find a suitable GPU!");
			}


			if (m_physicalDevice == VK_NULL_HANDLE)
			{
				EN_CORE_ERROR("Vulkan: Failed to find a suitable GPU!");
			}
		}

		bool VulkanDevice::IsDeviceSuitable(VkPhysicalDevice device, std::vector<const char*> deviceExtentions)
		{
			bool extensionSupport = CheckDeviceExtensionSupport(device, deviceExtentions);

			bool swapChainAdequate = false;
			if (extensionSupport)
			{
				SwapChainSupportDetails swapChainSupport = VulkanSwapchain::QuerySwapChainSupport(device);
				swapChainAdequate = !swapChainSupport.formats.empty() && !swapChainSupport.presentModes.empty();
			}

			return m_vkContext->GetVulkanQueue()->FindQueueFamilies(device).IsComplete() && extensionSupport && swapChainAdequate;
		}

		bool VulkanDevice::CheckDeviceExtensionSupport(VkPhysicalDevice device, std::vector<const char*> deviceExtentions)
		{
			uint32_t extensionCount;
			vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr);

			std::vector<VkExtensionProperties> availableExtensions(extensionCount);
			vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, availableExtensions.data());

			std::set<std::string> requiredExtensions(deviceExtentions.begin(), deviceExtentions.end());

			for (const auto& extension :availableExtensions)
			{
				requiredExtensions.erase(extension.extensionName);
			}

			return requiredExtensions.empty();
		}

		int VulkanDevice::RateDeviceSuitability(VkPhysicalDevice device)
		{
			VkPhysicalDeviceProperties deviceProperties;
			vkGetPhysicalDeviceProperties(device, &deviceProperties);

			VkPhysicalDeviceFeatures deviceFeatures;
			vkGetPhysicalDeviceFeatures(device, &deviceFeatures);

			int score = 0;

			// Discrete GPUs have a significant performance advantage
			if (deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU) {
				score += 1000;
			}

			// Maximum possible size of textures affects graphics quality
			score += deviceProperties.limits.maxImageDimension2D;

			// Application can't function without geometry shaders
			if (!deviceFeatures.geometryShader) 
			{
				return 0;
			}

			return score;
		}

		void VulkanDevice::CreateLogicalDevice(std::vector<const char*> deviceExtentions)
		{
			QueueFamilyIndices indices = m_vkContext->GetVulkanQueue()->FindQueueFamilies(m_physicalDevice);

			std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
			std::set<uint32_t> uniqueQueueFamilies = { indices.GraphicsFamily.value(), indices.PresentFamily.value() };

			float queuePriority = 1.0f;

			for (uint32_t queueFamily : uniqueQueueFamilies)
			{
				VkDeviceQueueCreateInfo queueCreateInfo = {};
				queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
				queueCreateInfo.queueFamilyIndex = queueFamily;
				queueCreateInfo.queueCount = 1;
				queueCreateInfo.pQueuePriorities = &queuePriority;
				queueCreateInfos.push_back(queueCreateInfo);
			}


			VkPhysicalDeviceFeatures deviceFeatures = {};

			VkDeviceCreateInfo createInfo = {};
			createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
			createInfo.pQueueCreateInfos = queueCreateInfos.data();
			createInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
			createInfo.pEnabledFeatures = &deviceFeatures;

			createInfo.enabledExtensionCount = static_cast<uint32_t>(deviceExtentions.size());
			createInfo.ppEnabledExtensionNames = deviceExtentions.data();

			if (m_vkContext->GetVulkanValidation()->GetValidationLayersState())
			{
				createInfo.enabledLayerCount = m_vkContext->GetVulkanValidation()->GetValidationLayerCount();
				createInfo.ppEnabledLayerNames = m_vkContext->GetVulkanValidation()->GetValidationLayers()->data();
			}
			else
			{
				createInfo.enabledLayerCount = 0;
			}

			if (vkCreateDevice(m_physicalDevice, &createInfo, nullptr, &m_device) != VK_SUCCESS)
			{
				EN_CORE_ERROR("Vulkan: Failed to create logical device!");
			}

			vkGetDeviceQueue(m_device, indices.GraphicsFamily.value(), 0, m_vkContext->GetVulkanQueue()->GetGraphicsQueue());
			vkGetDeviceQueue(m_device, indices.PresentFamily.value(), 0, m_vkContext->GetVulkanQueue()->GetPresentQueue());
		}
	}
}